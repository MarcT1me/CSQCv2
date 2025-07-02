#pragma once

#include "..\Resource\DX12Resource.h"
#include "..\Resource\DX12ResourceConfig.h"

namespace MirageAPI::DirectX
{
    ref class DX12DescriptorHeap;

    public ref class DX12FrameBuffer : public DX12Resource
    {
        UINT m_rtvDescriptorIndex = UINT_MAX;
        DX12DescriptorHeap^ m_rtvHeap;

    internal:
        DX12FrameBuffer(
            ID3D12Resource* resource,
            unsigned int width,
            unsigned int height,
            DX12DescriptorHeap^ rtvHeap,
            unsigned int rtvDescriptorIndex,
            DX12ResourceFormat format
        );

    public:
        DX12FrameBuffer(DX12ResourceConfig config);

        ~DX12FrameBuffer();
        !DX12FrameBuffer();

        void TransitionState(
            DX12CommandList^ commandList,
            DX12ResourceState newState
        ) override;

        virtual property DX12ResourceType ResourceType
        {
            DX12ResourceType get() override { return DX12ResourceType::FrameBuffer; }
        }

        property D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle
        {
            D3D12_CPU_DESCRIPTOR_HANDLE get();
        }
    };
}
