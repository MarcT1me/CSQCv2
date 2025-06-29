#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12FrameBuffer : public DX12Buffer
    {
        void* m_rtvHandlePtr;

    public:
        DX12FrameBuffer(
            ID3D12Resource* resource,
            D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle
        ) : DX12Buffer(resource),
            m_rtvHandlePtr(rtvHandle)
        {
        }

        ~DX12FrameBuffer();
        !DX12FrameBuffer();

        property D3D12_CPU_DESCRIPTOR_HANDLE* RTVHandle
        {
            D3D12_CPU_DESCRIPTOR_HANDLE* get() { return static_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(m_rtvHandlePtr); }
        }
    };
}
