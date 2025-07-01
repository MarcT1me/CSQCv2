#include "pch.h"
#include "DX12Texture.h"

#include "..\DX12Helpers.h"
#include "..\DX12Context.h"
#include "..\CommandList\DX12CommandList.h"

#include "DX12UploadBuffer.h"

namespace MirageAPI::DirectX
{
    DX12Texture::DX12Texture(
        DX12ResourceConfig config
    ) : DX12Resource(config.Width * config.Height, config.Format),
        m_type(config.TextureType),
        m_width(static_cast<UINT>(config.Width)),
        m_height(static_cast<UINT>(config.Height)),
        m_mipLevels(config.MipLevels),
        m_generateMipmaps(config.MipLevels > 1)
    {
        auto device = DX12Context::GetDevice();
        if (!device)
        {
            throw gcnew System::InvalidOperationException(
                "DirectX 12 device not initialized. Call DX12Context::Initialize() first."
            );
        }

        D3D12_HEAP_PROPERTIES heapProps = {
            static_cast<D3D12_HEAP_TYPE>(config.HeapType),
            D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
            D3D12_MEMORY_POOL_UNKNOWN,
            0, 0
        };

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        desc.Width = config.Width;
        desc.Height = config.Height;
        desc.DepthOrArraySize = config.Depth;
        desc.MipLevels = config.MipLevels;
        desc.Format = static_cast<DXGI_FORMAT>(config.Format);
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        desc.Flags = static_cast<D3D12_RESOURCE_FLAGS>(config.Flags);
        
        D3D12_CLEAR_VALUE* clearValuePtr = nullptr;
        D3D12_CLEAR_VALUE clearValue = {};

        if (static_cast<int>(config.Flags & DX12ResourceFlags::AllowRenderTarget) != 0)
        {
            clearValue.Format = static_cast<DXGI_FORMAT>(m_format);
            clearValue.Color[0] = 0.0f;
            clearValue.Color[1] = 0.0f;
            clearValue.Color[2] = 0.0f;
            clearValue.Color[3] = 1.0f;
            clearValuePtr = &clearValue;
        }
        else if (static_cast<int>(config.Flags & DX12ResourceFlags::AllowDepthStencil) != 0)
        {
            clearValue.Format = static_cast<DXGI_FORMAT>(m_format);
            clearValue.DepthStencil.Depth = 1.0f;
            clearValue.DepthStencil.Stencil = 0;
            clearValuePtr = &clearValue;
        }

        ID3D12Resource* buffer = nullptr;
        HRESULT hr = device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            static_cast<D3D12_RESOURCE_STATES>(config.InitialState),
            clearValuePtr,
            IID_PPV_ARGS(&buffer)
        );

        if (FAILED(hr))
        {
            throw gcnew System::Exception(
                "Failed to create buffer: " + hr
            );
        }

        m_nativeResource = buffer;
    }

    void DX12Texture::TransitionState(
        DX12CommandList^ commandList,
        DX12ResourceState newState)
    {
        if (m_currentState == newState) return;

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_nativeResource;
        barrier.Transition.StateBefore = static_cast<D3D12_RESOURCE_STATES>(m_currentState);
        barrier.Transition.StateAfter = static_cast<D3D12_RESOURCE_STATES>(newState);
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        commandList->NativeCommandList->ResourceBarrier(1, &barrier);
        m_currentState = newState;
    }

    void DX12Texture::UploadData(std::byte data[], int width, int height)
    {
        if (!m_nativeResource || !data || width <= 0 || height <= 0) return;

        const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_nativeResource, 0, 1);

        DX12ResourceConfig uploadConfig;
        uploadConfig.Type = DX12ResourceType::UploadBuffer;
        uploadConfig.Width = uploadBufferSize;
        uploadConfig.HeapType = DX12HeapType::Upload;
        uploadConfig.InitialState = DX12ResourceState::GenericRead;

        auto uploadBuffer = gcnew DX12UploadBuffer(uploadConfig);

        void* pData = uploadBuffer->Map();
        memcpy(pData, data, uploadBufferSize);
        uploadBuffer->Unmap();

        DX12CommandList^ commandList = gcnew DX12CommandList(DX12CommandListType::Direct);
        commandList->Reset();

        this->TransitionState(commandList, DX12ResourceState::CopyDest);

        D3D12_SUBRESOURCE_DATA subresourceData;
        subresourceData.pData = &data[0];
        subresourceData.RowPitch = static_cast<LONG_PTR>(width * GetTextureFormatSize(m_format));
        subresourceData.SlicePitch = subresourceData.RowPitch * height;

        UpdateSubresources(
            commandList->NativeCommandList,
            m_nativeResource,
            uploadBuffer->NativeResource,
            0, 0, 1,
            &subresourceData
        );

        // Возвращаем в исходное состояние
        this->TransitionState(commandList, DX12ResourceState::Common);

        // Выполняем команды
        commandList->Close();
        commandList->Execute();
        commandList->WaitForCompletion();

        delete commandList;
        delete uploadBuffer;

        if (m_generateMipmaps)
        {
            GenerateMipmaps(commandList);
        }
    }

    void DX12Texture::GenerateMipmaps(DX12CommandList^ commandList)
    {
        // Реализация генерации мипмапов будет добавлена позже
        // Это сложная операция, требующая вычислительных шейдеров
        // или последовательного рендеринга в уменьшающиеся текстуры
    }
}
