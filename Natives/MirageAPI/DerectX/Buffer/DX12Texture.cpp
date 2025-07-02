#include "pch.h"
#include "DX12Texture.h"

#include "..\DX12Context.h"
#include "DX12UploadBuffer.h"
#include "..\DX12Helpers.h"
#include "..\CommandList\DX12CommandList.h"

namespace MirageAPI::DirectX
{
    D3D12_RESOURCE_DIMENSION GetTextureDimension(DX12TextureType texType)
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

    DX12Texture::DX12Texture(
        DX12ResourceConfig config
    ) : DX12Resource(config, config.Width * config.Height * GetResourceFormatSize(config.Format)),
        m_textureType(config.TextureType),
        m_width(config.Width),
        m_height(config.Height),
        m_mipLevels(config.MipLevels)
    {
        auto device = DX12Context::GetDevice();
        if (!device)
        {
            throw gcnew System::InvalidOperationException(
                "DirectX 12 device not initialized. Call DX12Context::Initialize() first."
            );
        }

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = GetTextureDimension(m_textureType);
        desc.Width = m_width;
        desc.Height = m_height;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = m_mipLevels;
        desc.Format = static_cast<DXGI_FORMAT>(config.Format);
        desc.SampleDesc = {1, 0};
        desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        desc.Flags = static_cast<D3D12_RESOURCE_FLAGS>(config.Flags);

        D3D12_HEAP_PROPERTIES heapProps = {
            D3D12_HEAP_TYPE_DEFAULT,
            D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
            D3D12_MEMORY_POOL_UNKNOWN,
            0, 0
        };

        D3D12_CLEAR_VALUE* clearValuePtr = nullptr;

        ID3D12Resource* texture = nullptr;
        HRESULT hr = device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_COMMON,
            clearValuePtr,
            IID_PPV_ARGS(&texture)
        );
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            HRESULT reason = device->GetDeviceRemovedReason();
            throw gcnew System::Exception("Device removed during buffer creation: " + reason);
        }
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Failed to create buffer: " + hr);
        }
        m_nativeResource = texture;
    }

    DX12Texture::~DX12Texture()
    {
        this->!DX12Texture();
    }

    void DX12Texture::!DX12Texture()
    {
        ReleaseSRV();
    }

    void DX12Texture::TransitionState(
        DX12CommandList^ commandList,
        DX12ResourceState newState
    )
    {
    }

    void DX12Texture::UploadData(array<System::Byte>^ data)
    {
        if (!data || data->Length == 0)
        {
            throw gcnew System::Exception("Invalid texture data");
        }

        const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_nativeResource, 0, 1);

        // Проверка размера данных
        if (data->Length != m_size)
        {
            throw gcnew System::Exception(
                "Texture data size mismatch. Required: " + m_size + ", Actual: " + data->Length
            );
        }

        // Создаем временный upload буфер
        auto uploadBuffer = gcnew DX12UploadBuffer(uploadBufferSize);

        // Копируем данные в upload буфер
        void* pUploadData = uploadBuffer->Map();
        {
            pin_ptr<System::Byte> pinnedData = &data[0];

            // Копируем только актуальные данные без выравнивания
            const UINT expectedDataSize = m_width * m_height * GetResourceFormatSize(m_format);
            const size_t copySize = std::min(static_cast<size_t>(expectedDataSize),
                                             static_cast<size_t>(data->Length));

            memcpy(pUploadData, pinnedData, copySize);
        }
        uploadBuffer->Unmap();

        auto commandList = gcnew DX12CommandList(DX12CommandListType::Direct);
        commandList->Reset();

        // Барьер перехода
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_nativeResource;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        commandList->NativeCommandList->ResourceBarrier(1, &barrier);

        // Подготавливаем данные для копирования
        UINT rowPitch = m_width * GetResourceFormatSize(m_format);
        UINT slicePitch = rowPitch * m_height;

        // Автоматическое выравнивание
        if (rowPitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT != 0)
        {
            rowPitch = (rowPitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1) &
                ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1);
        }

        D3D12_SUBRESOURCE_DATA subresource_data;
        subresource_data.pData = pUploadData; // Используем данные из upload-буфера
        subresource_data.RowPitch = rowPitch;
        subresource_data.SlicePitch = slicePitch;

        // Копируем данные
        UpdateSubresources(
            commandList->NativeCommandList,
            m_nativeResource,
            uploadBuffer->NativeResource,
            0, 0, 1,
            &subresource_data
        );

        // Возвращаем в исходное состояние
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
        commandList->NativeCommandList->ResourceBarrier(1, &barrier);

        // Выполняем команды
        commandList->Close();
        commandList->Execute();
        commandList->WaitForCompletion();

        // Проверяем состояние устройства
        HRESULT hr = DX12Context::GetDevice()->GetDeviceRemovedReason();
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Device removed after texture upload: " + hr);
        }

        // Освобождаем ресурсы
        delete commandList;
        delete uploadBuffer;
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC DX12Texture::CreateSRVDesc()
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = static_cast<DXGI_FORMAT>(m_format);
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Texture2D.MipLevels = m_mipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.PlaneSlice = 0;
        srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
        return srvDesc;
    }

    void DX12Texture::CreateSRV()
    {
        if (m_srvIndex != UINT_MAX) return;

        auto srvHeap = DX12Context::GetDescriptorHeap(DX12DescriptorHeapType::CBV_SRV_UAV, 256, true);
        if (!srvHeap || !srvHeap->IsValid)
        {
            throw gcnew System::InvalidOperationException("Invalid SRV descriptor heap");
        }

        m_srvIndex = srvHeap->Allocate();
        auto device = DX12Context::GetDevice();

        D3D12_CPU_DESCRIPTOR_HANDLE handle = srvHeap->NativeHeap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += m_srvIndex * srvHeap->DescriptorSize;

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = CreateSRVDesc();
        device->CreateShaderResourceView(
            m_nativeResource,
            &desc,
            handle
        );
    }

    void DX12Texture::ReleaseSRV()
    {
        if (m_srvIndex == UINT_MAX) return;

        auto srvHeap = DX12Context::GetDescriptorHeap(DX12DescriptorHeapType::CBV_SRV_UAV, 256, true);

        srvHeap->Free(m_srvIndex);
        m_srvIndex = UINT_MAX;
    }
}
