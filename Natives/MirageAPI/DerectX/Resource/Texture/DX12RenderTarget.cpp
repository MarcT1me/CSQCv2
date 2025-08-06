#include "pch.h"
#include "DX12RenderTarget.h"

#include "../../Command/DX12CommandList.h"

namespace MirageAPI::DirectX::Resource
{
    DX12RenderTarget::DX12RenderTarget(
        DX12ResourceConfig config,
        ID3D12Resource* resource,
        DX12DescriptorHeap^ rtvHeap
    ) : DX12Resource(config),
        RTVHeap(rtvHeap)
    {
        m_nativeResource = resource;
    }

    DX12RenderTarget::DX12RenderTarget(
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

        if (config.Flags.HasFlag(DX12ResourceFlags::AllowRenderTarget))
        {
            clearValue.Format = static_cast<DXGI_FORMAT>(m_resourceFormat);
            clearValue.Color[0] = 0.0f;
            clearValue.Color[1] = 0.0f;
            clearValue.Color[2] = 0.0f;
            clearValue.Color[3] = 1.0f;
            clearValuePtr = &clearValue;
        }
        else if (config.Flags.HasFlag(DX12ResourceFlags::AllowDepthStencil))
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

    DX12RenderTarget::!DX12RenderTarget()
    {
        Validate();

        ReleaseRTV();
    }

    void DX12RenderTarget::TransitionState(
        Command::DX12CommandList^ commandList,
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

    void DX12RenderTarget::CreateRTV()
    {
        CheckMissmatch(m_rtvDescriptorIndex, UINT_MAX) return;

        m_rtvDescriptorIndex = RTVHeap->Allocate();

        device->CreateRenderTargetView(
            m_nativeResource,
            nullptr,
            RTVHandleForCPU
        );
    }

    void DX12RenderTarget::ReleaseRTV()
    {
        CheckMissmatch(m_rtvDescriptorIndex, UINT_MAX) return;

        RTVHeap->Free(m_rtvDescriptorIndex);
        m_rtvDescriptorIndex = UINT_MAX;
    }
}
