#include "pch.h"
#include "DX12Buffer.h"

#include "..\DX12Context.h"
#include "..\CommandList\DX12CommandList.h"

namespace MirageAPI::DirectX
{
    DX12Buffer::DX12Buffer(
        DX12ResourceConfig config
    ) : DX12Resource(config, config.Width * config.Stride),
        m_elementCount(config.Width),
        m_stride(config.Stride)
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
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width = config.Width;
        desc.Format = static_cast<DXGI_FORMAT>(config.Format);
        desc.Flags = static_cast<D3D12_RESOURCE_FLAGS>(config.Flags);

        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.SampleDesc = {1, 0};
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        ID3D12Resource* buffer = nullptr;
        HRESULT hr = device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            static_cast<D3D12_RESOURCE_STATES>(config.InitialState),
            nullptr,
            IID_PPV_ARGS(&buffer)
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
        m_nativeResource = buffer;
    }

    void DX12Buffer::TransitionState(
        DX12CommandList^ commandList,
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

        commandList->NativeCommandList->ResourceBarrier(1, &barrier);
        m_currentState = newState;
    }

    void DX12Buffer::UploadData(
        array<System::Byte>^ data
    )
    {
        if (!m_nativeResource || data->Length != m_size) return;

        if (void* pData = this->Map())
        {
            pin_ptr<System::Byte> pinData = &data[0];
            memcpy(pData, pinData, m_size);
            Unmap();
        }
    }
}
