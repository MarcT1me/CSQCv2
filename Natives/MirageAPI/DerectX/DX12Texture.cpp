#include "pch.h"
#include "DX12Texture.h"

#include <cstddef>

#include "DX12Buffer.h"
#include "DX12CommandList.h"
#include "DX12Helpers.h"

namespace MirageAPI::DirectX
{
    DX12Texture::DX12Texture(
        int width,
        int height,
        DX12TextureFormat format,
        DX12TextureType type,
        bool generateMipmaps
    ) : m_type(type),
        m_format(static_cast<DXGI_FORMAT>(format)),
        m_currentState(D3D12_RESOURCE_STATE_COMMON)
    {
        m_desc = {};
        m_desc->Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        m_desc->Width = width;
        m_desc->Height = height;
        m_desc->DepthOrArraySize = (type == DX12TextureType::CubeMap) ? 6 : 1;
        m_desc->MipLevels = generateMipmaps ? 0 : 1;
        m_desc->Format = static_cast<DXGI_FORMAT>(format);
        m_desc->SampleDesc.Count = 1;
        m_desc->SampleDesc.Quality = 0;
        m_desc->Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        m_desc->Flags = D3D12_RESOURCE_FLAG_NONE;

        if (type == DX12TextureType::RenderTarget)
            m_desc->Flags = m_desc->Flags | D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
        else if (type == DX12TextureType::DepthStencil)
            m_desc->Flags = m_desc->Flags | D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    }

    DX12Texture::~DX12Texture() { this->!DX12Texture(); }
    DX12Texture::!DX12Texture() { if (m_texture) m_texture->Release(); }

    void DX12Texture::Create()
    {
        auto device = DX12Context::GetDevice();

        D3D12_HEAP_PROPERTIES heapProps;
        heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;

        D3D12_CLEAR_VALUE clearValue = {};
        DXGI_FORMAT format = m_format;
        clearValue.Format = format;

        if (m_type == DX12TextureType::RenderTarget)
        {
            clearValue.Color[0] = 0.0f;
            clearValue.Color[1] = 0.0f;
            clearValue.Color[2] = 0.0f;
            clearValue.Color[3] = 1.0f;
        }
        else if (m_type == DX12TextureType::DepthStencil)
        {
            clearValue.DepthStencil.Depth = 1.0f;
            clearValue.DepthStencil.Stencil = 0;
        }

        ID3D12Resource* texture;
        HRESULT hr = device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            m_desc,
            m_currentState,
            m_type == DX12TextureType::RenderTarget || m_type == DX12TextureType::DepthStencil
                ? &clearValue
                : nullptr,
            IID_PPV_ARGS(&texture)
        );
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Failed to create texture resource");
        }
        m_texture = texture;
    }

    void DX12Texture::UploadData(std::byte data[], int size, int mipLevel)
    {
        if (!m_texture || !data) return;

        const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture, 0, 1);

        DX12Buffer^ uploadBuffer = gcnew DX12Buffer(
            static_cast<UINT>(uploadBufferSize),
            1,
            DX12BufferType::Upload
        );

        void* pData = uploadBuffer->Map();
        memcpy(pData, data, size);
        uploadBuffer->Unmap();

        DX12CommandList^ commandList = gcnew DX12CommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
        commandList->Reset();

        TransitionState(commandList, D3D12_RESOURCE_STATE_COPY_DEST);

        D3D12_SUBRESOURCE_DATA subresourceData;
        subresourceData.pData = &data[0];
        subresourceData.RowPitch = static_cast<LONG_PTR>(m_desc->Width * 4); // Для RGBA8
        subresourceData.SlicePitch = subresourceData.RowPitch * m_desc->Height;

        UpdateSubresources(
            commandList->NativeCommandList,
            m_texture,
            uploadBuffer->NativeResource,
            0,
            0,
            1,
            &subresourceData
        );

        // Возвращаем в исходное состояние
        TransitionState(commandList, D3D12_RESOURCE_STATE_COMMON);

        // Выполняем команды
        commandList->Close();
        commandList->Execute();
        commandList->WaitForCompletion();

        delete uploadBuffer;
        delete commandList;
    }

    void DX12Texture::TransitionState(
        DX12CommandList^ commandList,
        D3D12_RESOURCE_STATES newState)
    {
        if (!commandList || !m_texture || m_currentState == newState) return;

        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = m_texture;
        D3D12_RESOURCE_STATES currentState = m_currentState;
        barrier.Transition.StateBefore = currentState;
        barrier.Transition.StateAfter = newState;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        commandList->NativeCommandList->ResourceBarrier(1, &barrier);
        m_currentState = newState;
    }
}
