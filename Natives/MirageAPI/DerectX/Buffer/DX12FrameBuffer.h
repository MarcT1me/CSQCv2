#pragma once

#include "..\Resource\DX12Resource.h"
#include "..\Resource\DX12ResourceConfig.h"

namespace MirageAPI::DirectX
{
    public ref class DX12FrameBuffer : public DX12Resource
    {
        D3D12_CPU_DESCRIPTOR_HANDLE* m_rtvHandle;

    internal:
        DX12FrameBuffer(
            ID3D12Resource* resource,
            UINT size,
            D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle,
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

        property D3D12_CPU_DESCRIPTOR_HANDLE* RTVHandle
        {
            D3D12_CPU_DESCRIPTOR_HANDLE* get() { return m_rtvHandle; }
        }
    };
}
