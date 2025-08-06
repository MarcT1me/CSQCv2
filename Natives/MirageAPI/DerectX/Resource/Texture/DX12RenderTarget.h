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
    public ref class DX12RenderTarget : public DX12Resource
    {
        // heap
        UINT m_rtvDescriptorIndex = UINT_MAX;

    internal:
        // constructors and deconstructors
        DX12RenderTarget(
            DX12ResourceConfig config,
            ID3D12Resource* resource,
            DX12DescriptorHeap^ rtvHeap
        );

    public:
        DX12DescriptorHeap^ RTVHeap;

        DX12RenderTarget(DX12ResourceConfig config);

        ~DX12RenderTarget() { this->!DX12RenderTarget(); }
        !DX12RenderTarget();

        // other properties
        property UINT DescriptorIndex
        {
            UINT get() { return m_rtvDescriptorIndex; }
        }
        property D3D12_CPU_DESCRIPTOR_HANDLE RTVHandleForCPU
        {
            D3D12_CPU_DESCRIPTOR_HANDLE get() { return RTVHeap->IndexCPUHandle(m_rtvDescriptorIndex); }
        }
        property D3D12_GPU_DESCRIPTOR_HANDLE RTVHandleForGPU
        {
            D3D12_GPU_DESCRIPTOR_HANDLE get() { return RTVHeap->IndexGPUHandle(m_rtvDescriptorIndex); }
        }
        property UINT RTVIndex { UINT get() { return m_rtvDescriptorIndex; } }
        property bool HasRTV { bool get() { return m_rtvDescriptorIndex != UINT_MAX; } }

        // buffer operations
        virtual void TransitionState(
            Command::DX12CommandList^ commandList,
            DX12ResourceState newState
        );

        void CreateRTV();
        void ReleaseRTV();
    };
}
