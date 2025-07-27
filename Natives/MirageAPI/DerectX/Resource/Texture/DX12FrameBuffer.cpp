#include "pch.h"
#include "DX12FrameBuffer.h"

#include "../../DX12DescriptorHeap.h"
#include "../../CommandList/DX12CommandList.h"

namespace MirageAPI::DirectX::Resource
{
    DX12FrameBuffer::DX12FrameBuffer(
        ID3D12Resource* resource,
        UINT width,
        UINT height,
        DX12ResourceFormat format,
        DX12DescriptorHeap^ rtvHeap
    ) : DX12Resource(
            DX12ResourceConfig::RenderTargetConfig(width, height, format)
        ),
        m_rtvHeap(rtvHeap)
    {
        m_nativeResource = resource;
    }

    DX12FrameBuffer::DX12FrameBuffer(
        DX12ResourceConfig config
    ) : DX12Resource(config)
    {
        D3D12_HEAP_PROPERTIES heapProps = {
            D3D12_HEAP_TYPE_DEFAULT,
            D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
            D3D12_MEMORY_POOL_UNKNOWN,
            0, 0
        };

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        desc.Width = config.Width;
        desc.Height = config.Height;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = static_cast<DXGI_FORMAT>(m_resourceFormat);
        desc.SampleDesc = {1, 0};
        desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

        D3D12_CLEAR_VALUE* clearValuePtr = nullptr;
        D3D12_CLEAR_VALUE clearValue = {};

        if (static_cast<int>(config.Flags & DX12ResourceFlags::AllowRenderTarget) != 0)
        {
            clearValue.Format = static_cast<DXGI_FORMAT>(m_resourceFormat);
            clearValue.Color[0] = 0.0f;
            clearValue.Color[1] = 0.0f;
            clearValue.Color[2] = 0.0f;
            clearValue.Color[3] = 1.0f;
            clearValuePtr = &clearValue;
        }
        else if (static_cast<int>(config.Flags & DX12ResourceFlags::AllowDepthStencil) != 0)
        {
            clearValue.Format = static_cast<DXGI_FORMAT>(m_resourceFormat);
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
        CheckHResult(hr, "Failed to create buffer");
        m_nativeResource = buffer;
    }

    DX12FrameBuffer::!DX12FrameBuffer()
    {
        if (m_rtvDescriptorIndex != UINT_MAX && m_rtvHeap != nullptr)
        {
            m_rtvHeap->Free(m_rtvDescriptorIndex);
            m_rtvDescriptorIndex = UINT_MAX;
        }
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12FrameBuffer::RTVHandle::get()
    {
        m_rtvHeap->Validate();

        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeap->StartCPUHandle;
        handle.ptr += m_rtvDescriptorIndex * m_rtvHeap->DescriptorSize;
        return handle;
    }

    void DX12FrameBuffer::TransitionState(
        CommandList::DX12CommandList^ commandList,
        DX12ResourceState newState
    )
    {
        if (m_currentState == newState) return;

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = m_nativeResource;
        barrier.Transition.StateBefore = static_cast<D3D12_RESOURCE_STATES>(m_currentState);
        barrier.Transition.StateAfter = static_cast<D3D12_RESOURCE_STATES>(newState);
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        commandList->NativeList->ResourceBarrier(1, &barrier);
        m_currentState = newState;
    }
    
    const D3D12_RENDER_TARGET_VIEW_DESC* DX12FrameBuffer::CreateRTVDesc()
    {
        return nullptr;
    }

    void DX12FrameBuffer::CreateRTV()
    {
        if (m_rtvDescriptorIndex != UINT_MAX) return;

        m_rtvHeap->Validate();

        m_rtvDescriptorIndex = m_rtvHeap->Allocate();
        
        device->CreateRenderTargetView(
            m_nativeResource,
            CreateRTVDesc(),
            RTVHandle
        );
    }

    void DX12FrameBuffer::ReleaseRTV()
    {
        if (m_rtvDescriptorIndex == UINT_MAX) return;

        m_rtvHeap->Free(m_rtvDescriptorIndex);
        m_rtvDescriptorIndex = UINT_MAX;
    }
}
