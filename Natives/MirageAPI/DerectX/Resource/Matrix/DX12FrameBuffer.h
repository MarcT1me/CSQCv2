#pragma once

#include "../DX12Resource.h"
#include "../../DX12DescriptorHeap.h"

namespace MirageAPI::DirectX
{
    ref class DX12DescriptorHeap;
}

namespace MirageAPI::DirectX::Command
{
    ref class DX12CommandList;
}

namespace MirageAPI::DirectX::Resource
{
    public ref class DX12FrameBuffer : public DX12Resource
    {
        // heap
        DX12DescriptorHeap^ m_rtvHeap;
        UINT m_rtvDescriptorIndex = UINT_MAX;

    internal:
        // constructors and deconstructors
        DX12FrameBuffer(
            ID3D12Resource* resource,
            UINT width,
            UINT height,
            DX12ResourceFormat format,
            DX12DescriptorHeap^ rtvHeap
        );

    public:
        DX12FrameBuffer(DX12ResourceConfig config);

        ~DX12FrameBuffer() { this->!DX12FrameBuffer(); }
        !DX12FrameBuffer();

        // other properties
        property UINT DescriptorIndex
        {
            UINT get() { return m_rtvDescriptorIndex; }
        }
        property DX12DescriptorHeap^ RTVHeap
        {
            DX12DescriptorHeap^ get() { return m_rtvHeap; }
            void set(DX12DescriptorHeap^ value) { m_rtvHeap = value; }
        }
        property D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle
        {
            D3D12_CPU_DESCRIPTOR_HANDLE get() { return m_rtvHeap->IndexCPUHandle(m_rtvDescriptorIndex); }
        }

        // buffer operations
        virtual void TransitionState(
            Command::DX12CommandList^ commandList,
            DX12ResourceState newState
        );

        void CreateRTV();
        void ReleaseRTV();
    };
}
