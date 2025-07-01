#include "pch.h"
#include "DX12FrameBuffer.h"

#include "..\DX12Context.h"
#include "..\CommandList\DX12CommandList.h"

namespace MirageAPI::DirectX
{
    DX12FrameBuffer::DX12FrameBuffer(
        ID3D12Resource* resource,
        UINT size,
        D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle,
        DX12ResourceFormat format
    ) : DX12Resource(size, format),
        m_rtvHandle(rtvHandle)
    {
        m_nativeResource = resource;
        m_currentState = DX12ResourceState::Present;
    }

    DX12FrameBuffer::DX12FrameBuffer(
        DX12ResourceConfig config
    ) : DX12Resource(config.Width * config.Height * 4, config.Format)
    {
        auto device = DX12Context::GetDevice();
        if (!device)
            throw gcnew System::Exception("DX12 device not initialized");

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
        desc.Format = static_cast<DXGI_FORMAT>(m_format);
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

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

        // Создаем RTV
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = 1;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        ID3D12DescriptorHeap* rtvHeap;
        if (FAILED(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap))))
        {
            throw gcnew System::Exception("Failed to create RTV heap");
        }

        m_rtvHandle = new D3D12_CPU_DESCRIPTOR_HANDLE();
        *m_rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
        device->CreateRenderTargetView(m_nativeResource, nullptr, *m_rtvHandle);
    }

    DX12FrameBuffer::~DX12FrameBuffer()
    {
        this->!DX12FrameBuffer();
    }

    DX12FrameBuffer::!DX12FrameBuffer()
    {
        if (m_rtvHandle)
        {
            delete m_rtvHandle;
            m_rtvHandle = nullptr;
        }

        if (m_nativeResource)
        {
            Unmap();
            if (m_nativeResource->Release() == 0)
            {
                m_nativeResource = nullptr;
            }
        }
    }

    void DX12FrameBuffer::TransitionState(
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
}
