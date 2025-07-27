#include "pch.h"
#include "DX12Texture.h"

#include "../../DX12DescriptorHeap.h"
#include "../Buffer/DX12UploadBuffer.h"
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
        D3D12_RESOURCE_DESC desc = destinationResource->GetDesc();
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
        desc.DepthOrArraySize = config.Depth;
        desc.MipLevels = m_mipLevels;
        desc.Format = static_cast<DXGI_FORMAT>(config.Format);
        desc.Flags = static_cast<D3D12_RESOURCE_FLAGS>(config.Flags);
        // constant
        desc.SampleDesc = {1, 0};
        desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

        D3D12_CLEAR_VALUE* clearValuePtr = nullptr;
        // TODO: clear value

        // creating texture himself
        ID3D12Resource* texture = nullptr;
        CheckHResult(
            device->CreateCommittedResource(
                &heapProps,
                D3D12_HEAP_FLAG_NONE,
                &desc,
                D3D12_RESOURCE_STATE_COMMON,
                clearValuePtr,
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

    D3D12_CPU_DESCRIPTOR_HANDLE DX12Texture::SRVHandle::get()
    {
        m_srvHeap->Validate();

        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_srvHeap->StartCPUHandle;
        handle.ptr += m_srvDescriptorIndex * m_srvHeap->DescriptorSize;
        return handle;
    }

    // texture operations

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

        // creating one-time upload buffer
        auto uploadBuffer = gcnew DX12UploadBuffer(uploadBufferSize);

        // copying into upload buffer
        {
            void* pUploadData = uploadBuffer->Map();
            pin_ptr<System::Byte> pinnedData = &data[0];

            const size_t copySize = std::min(
                static_cast<size_t>(m_size),
                static_cast<size_t>(data->Length)
            );

            memcpy(pUploadData, pinnedData, copySize);
            uploadBuffer->Unmap();
        }

        auto commandList = gcnew CommandList::DX12CommandList(DX12CommandListType::Direct);
        commandList->Reset();

        // barrier
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_nativeResource;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        commandList->NativeList->ResourceBarrier(1, &barrier);

        // Копируем данные
        UpdateSubresources(
            commandList->NativeList,
            m_nativeResource,
            uploadBuffer->NativeResource,
            0, 0, 1
        );

        // Возвращаем в исходное состояние
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        commandList->NativeList->ResourceBarrier(1, &barrier);

        // Выполняем команды
        commandList->Close();
        commandList->Execute();
        commandList->WaitForCompletion();

        // Проверяем состояние устройства
        HRESULT hr = device->GetDeviceRemovedReason();
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Device removed after texture upload: " + hr);
        }

        // Освобождаем ресурсы
        delete commandList;
        delete uploadBuffer;
    }

    const D3D12_SHADER_RESOURCE_VIEW_DESC* DX12Texture::CreateSRVDesc()
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = static_cast<DXGI_FORMAT>(m_resourceFormat);
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Texture2D.MipLevels = m_mipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.PlaneSlice = 0;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
        return &srvDesc;
    }

    void DX12Texture::CreateSRV()
    {
        if (m_srvDescriptorIndex != UINT_MAX) return;

        m_srvHeap->Validate();

        m_srvDescriptorIndex = m_srvHeap->Allocate();
        
        device->CreateShaderResourceView(
            m_nativeResource,
            CreateSRVDesc(),
            SRVHandle
        );
    }

    void DX12Texture::ReleaseSRV()
    {
        if (m_srvDescriptorIndex == UINT_MAX) return;

        m_srvHeap->Free(m_srvDescriptorIndex);
        m_srvDescriptorIndex = UINT_MAX;
    }
}
