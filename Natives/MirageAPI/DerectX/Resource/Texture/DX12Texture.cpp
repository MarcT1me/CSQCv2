#include "pch.h"
#include "DX12Texture.h"

#include "../../DX12DescriptorHeap.h"
#include "../Buffer/DX12UploadBuffer.h"
#include "../../CommandList/DX12CommandQueue.h"
#include "../../CommandList/DX12CommandList.h"

namespace MirageAPI::DirectX::Resource
{
    inline D3D12_RESOURCE_DIMENSION GetTextureDimension(DX12TextureType texType)
    {
        switch (texType)
        {
        case DX12TextureType::Texture1D:
            return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
        case DX12TextureType::Texture2D:
        case DX12TextureType::DepthStencil:
            return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        case DX12TextureType::Texture3D:
            return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
        default:
            throw gcnew System::ArgumentException("Unknown texture type");
        }
    }

    inline UINT64 GetRequiredIntermediateSize(
        ID3D12Resource* destinationResource,
        UINT firstSubresource,
        UINT numSubresources
    )
    {
        const auto desc = destinationResource->GetDesc();
        UINT64 requiredSize = 0;

        ID3D12Device* device;
        destinationResource->GetDevice(IID_PPV_ARGS(&device));
        device->GetCopyableFootprints(
            &desc, firstSubresource, numSubresources, 0,
            nullptr, nullptr, nullptr, &requiredSize
        );
        device->Release();

        return requiredSize;
    }

    DX12Texture::DX12Texture(
        DX12ResourceConfig config
    ) : DX12Resource(config),
        m_textureType(config.TextureType),
        m_width(config.Width),
        m_height(config.Height),
        m_depth(config.Depth),
        m_mipLevels(config.MipLevels)
    {
        // creating heap info
        D3D12_HEAP_PROPERTIES heapProps = {
            D3D12_HEAP_TYPE_DEFAULT,
            D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
            D3D12_MEMORY_POOL_UNKNOWN,
            0, 0
        };

        // creating resource description
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = GetTextureDimension(m_textureType);
        desc.Width = m_width;
        desc.Height = m_height;
        desc.DepthOrArraySize = m_depth;
        desc.MipLevels = m_mipLevels;
        desc.Format = static_cast<DXGI_FORMAT>(config.Format);
        desc.Flags = static_cast<D3D12_RESOURCE_FLAGS>(config.Flags);
        // constant
        desc.SampleDesc = {1, 0};

        // creating texture himself
        ID3D12Resource* texture = nullptr;
        CheckHResult(
            device->CreateCommittedResource(
                &heapProps,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_COPY_DEST,
                nullptr,
                IID_PPV_ARGS(&texture)
            ),
            "Failed to create texture"
        );
        m_nativeResource = texture;
    }

    void DX12Texture::!DX12Texture()
    {
        ReleaseSRV();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12Texture::SRVHandleForCPU::get()
    {
        m_srvHeap->Validate();

        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_srvHeap->StartCPUHandle;
        handle.ptr += m_srvDescriptorIndex * m_srvHeap->DescriptorSize;
        return handle;
    }

    D3D12_GPU_DESCRIPTOR_HANDLE DX12Texture::SRVHandleForGPU::get()
    {
        m_srvHeap->Validate();

        D3D12_GPU_DESCRIPTOR_HANDLE handle = m_srvHeap->StartGPUHandle;
        handle.ptr += m_srvDescriptorIndex * m_srvHeap->DescriptorSize;
        return handle;
    }

    // texture operations

    inline void MemcpySubresource(
        _In_ const D3D12_MEMCPY_DEST* pDest,
        _In_ const D3D12_SUBRESOURCE_DATA* pSrc,
        SIZE_T RowSizeInBytes,
        UINT NumRows,
        UINT NumSlices
    ) noexcept
    {
        for (UINT z = 0; z < NumSlices; ++z)
        {
            auto pDestSlice = static_cast<BYTE*>(pDest->pData) + pDest->SlicePitch * z;
            auto pSrcSlice = static_cast<const BYTE*>(pSrc->pData) + pSrc->SlicePitch * static_cast<LONG_PTR>(z);
            for (UINT y = 0; y < NumRows; ++y)
            {
                memcpy(pDestSlice + pDest->RowPitch * y,
                       pSrcSlice + pSrc->RowPitch * static_cast<LONG_PTR>(y),
                       RowSizeInBytes);
            }
        }
    }

    inline UINT64 UpdateSubresources(
        _In_ ID3D12GraphicsCommandList* pCmdList,
        _In_ ID3D12Resource* pDestinationResource,
        _In_ ID3D12Resource* pIntermediate,
        _In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
        _In_range_(0, D3D12_REQ_SUBRESOURCES-FirstSubresource) UINT NumSubresources,
        UINT64 RequiredSize,
        _In_reads_(NumSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
        _In_reads_(NumSubresources) const UINT* pNumRows,
        _In_reads_(NumSubresources) const UINT64* pRowSizesInBytes,
        _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData
    ) noexcept
    {
        const auto IntermediateDesc = pIntermediate->GetDesc();
        const auto DestinationDesc = pDestinationResource->GetDesc();

        if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER ||
            IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset ||
            RequiredSize > static_cast<SIZE_T>(-1) ||
            (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&
                (FirstSubresource != 0 || NumSubresources != 1)))
        {
            return 0;
        }

        BYTE* pData;
        HRESULT hr = pIntermediate->Map(0, nullptr, reinterpret_cast<void**>(&pData));
        if (FAILED(hr))
        {
            return 0;
        }

        for (UINT i = 0; i < NumSubresources; ++i)
        {
            if (pRowSizesInBytes[i] > static_cast<SIZE_T>(-1)) return 0;
            D3D12_MEMCPY_DEST DestData = {
                pData + pLayouts[i].Offset, pLayouts[i].Footprint.RowPitch,
                static_cast<SIZE_T>(pLayouts[i].Footprint.RowPitch) * static_cast<SIZE_T>(pNumRows[i])
            };
            MemcpySubresource(&DestData, &pSrcData[i], static_cast<SIZE_T>(pRowSizesInBytes[i]), pNumRows[i],
                              pLayouts[i].Footprint.Depth);
        }
        pIntermediate->Unmap(0, nullptr);

        if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
        {
            pCmdList->CopyBufferRegion(
                pDestinationResource, 0, pIntermediate, pLayouts[0].Offset, pLayouts[0].Footprint.Width);
        }
        else
        {
            for (UINT i = 0; i < NumSubresources; ++i)
            {
                UINT srsIndex = i + FirstSubresource;
                D3D12_TEXTURE_COPY_LOCATION dst = {};
                dst.pResource = pDestinationResource;
                dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                dst.PlacedFootprint = {};
                dst.SubresourceIndex = srsIndex;

                D3D12_TEXTURE_COPY_LOCATION src = {};
                src.pResource = pIntermediate;
                src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
                src.PlacedFootprint = pLayouts[i];

                pCmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
            }
        }
        return RequiredSize;
    }

    inline UINT64 UpdateSubresources(
        _In_ ID3D12GraphicsCommandList* pCmdList,
        _In_ ID3D12Resource* pDestinationResource,
        _In_ ID3D12Resource* pIntermediate,
        UINT64 IntermediateOffset,
        _In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
        _In_range_(0, D3D12_REQ_SUBRESOURCES-FirstSubresource) UINT NumSubresources,
        _In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData
    ) noexcept
    {
        UINT64 RequiredSize = 0;
        const auto MemToAlloc = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(
            UINT64)) * NumSubresources;
        if (MemToAlloc > SIZE_MAX)
        {
            return 0;
        }
        void* pMem = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(MemToAlloc));
        if (pMem == nullptr)
        {
            return 0;
        }
        auto pLayouts = static_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(pMem);
        auto pRowSizesInBytes = reinterpret_cast<UINT64*>(pLayouts + NumSubresources);
        auto pNumRows = reinterpret_cast<UINT*>(pRowSizesInBytes + NumSubresources);

        const auto Desc = pDestinationResource->GetDesc();

        ID3D12Device* pDevice = nullptr;
        pDestinationResource->GetDevice(IID_PPV_ARGS(&pDevice));
        pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, IntermediateOffset, pLayouts, pNumRows,
                                       pRowSizesInBytes, &RequiredSize);
        pDevice->Release();

        const UINT64 Result = UpdateSubresources(pCmdList, pDestinationResource, pIntermediate, FirstSubresource,
                                                 NumSubresources, RequiredSize, pLayouts, pNumRows, pRowSizesInBytes,
                                                 pSrcData);
        HeapFree(GetProcessHeap(), 0, pMem);
        return Result;
    }

    void DX12Texture::UploadData(array<System::Byte>^ data)
    {
        // validate texture himself and data
        Validate();
        if (!data || data->Length == 0)
            throw gcnew System::ArgumentException("Invalid texture data");

        const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_nativeResource, 0, 1);

        // check data sizes
        if (data->Length != m_size)
        {
            throw gcnew System::Exception(
                "Texture data size mismatch. Required: " + m_size + ", Actual: " + data->Length
            );
        }

        // create lists for texture data copy operations
        auto commandQueue = gcnew CommandList::DX12CommandQueue(DX12CommandListType::Direct);
        auto commandList = gcnew CommandList::DX12CommandList(DX12CommandListType::Direct);
        auto fence = gcnew DX12Fence(0);
        commandQueue->Fence = fence;

        // creating one-time upload buffer
        auto uploadBuffer = gcnew DX12UploadBuffer(uploadBufferSize);

        // subresource
        {
            const size_t copySize = std::min(
                static_cast<size_t>(m_size),
                static_cast<size_t>(data->Length)
            );

            void* pUploadData = malloc(copySize);
            pin_ptr<System::Byte> pinnedData = &data[0];
            memcpy(pUploadData, pinnedData, copySize);

            D3D12_SUBRESOURCE_DATA textureSubresourceData;
            textureSubresourceData.pData = pUploadData;
            textureSubresourceData.RowPitch = m_width * GetResourceFormatSize(m_resourceFormat);
            textureSubresourceData.SlicePitch = textureSubresourceData.RowPitch * m_height;

            // reset cmd list
            commandQueue->Signal();
            fence->WaitForCompletion();
            fence->IncreaseValue();

            commandList->Reset();

            // update subresources
            UpdateSubresources(
                commandList->NativeList,
                m_nativeResource,
                uploadBuffer->NativeResource,
                0, 0, 1,
                &textureSubresourceData
            );
        }

        // change to shader resource
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_nativeResource;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        commandList->NativeList->ResourceBarrier(1, &barrier);

        // execute commands
        commandList->Close();

        commandQueue->ExecuteList(commandList);
        commandQueue->WaitForCompletion();
        commandQueue->Signal();

        fence->WaitForCompletion();
        fence->IncreaseValue();

        // checking device errors
        HRESULT hr = device->GetDeviceRemovedReason();
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Device removed after texture upload: " + hr);
        }

        // free used resources
        delete commandList;
        delete commandQueue;
        delete uploadBuffer;
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC DX12Texture::CreateSRVDesc()
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = static_cast<DXGI_FORMAT>(m_resourceFormat);
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Texture2D.MipLevels = m_mipLevels;
        return srvDesc;
    }

    void DX12Texture::CreateSRV()
    {
        if (m_srvDescriptorIndex != UINT_MAX) return;

        m_srvHeap->Validate();

        m_srvDescriptorIndex = m_srvHeap->Allocate();

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = CreateSRVDesc();
        device->CreateShaderResourceView(
            m_nativeResource,
            &desc,
            SRVHandleForCPU
        );
    }

    void DX12Texture::ReleaseSRV()
    {
        if (m_srvDescriptorIndex == UINT_MAX) return;

        m_srvHeap->Free(m_srvDescriptorIndex);
        m_srvDescriptorIndex = UINT_MAX;
    }
}
