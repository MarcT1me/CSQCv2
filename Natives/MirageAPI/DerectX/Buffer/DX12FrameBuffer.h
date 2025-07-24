#pragma once

#include "..\Resource\DX12Resource.h"
#include "..\Resource\DX12ResourceConfig.h"

namespace MirageAPI::DirectX
{
    ref class DX12CommandList;
    ref class DX12DescriptorHeap;

    public ref class DX12FrameBuffer : public DX12Resource
    {
    internal:
        // heap
        DX12DescriptorHeap^ m_rtvHeap;
        UINT m_rtvDescriptorIndex = UINT_MAX;

        // constructors and deconstructors
        DX12FrameBuffer(
            ID3D12Resource* resource,
            UINT width,
            UINT height,
            DX12ResourceFormat format,
            DX12DescriptorHeap^ rtvHeap,
            UINT rtvDescriptorIndex
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
            D3D12_CPU_DESCRIPTOR_HANDLE get();
        }

        // buffer operations
        virtual void TransitionState(
            DX12CommandList^ commandList,
            DX12ResourceState newState
        );
    };
}
