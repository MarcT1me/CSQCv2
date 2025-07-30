#pragma once

#include "DX12CommandList.h"
#include "DX12Fence.h"

namespace MirageAPI::DirectX::Command
{
    public ref class DX12CommandQueue : public DX12Object
    {
        ID3D12CommandQueue* m_commandQueue = nullptr;
        // descriptions
        DX12CommandListType m_type;
        // fence
        DX12Fence^ m_fence = nullptr;

    public:
        DX12CommandQueue(DX12CommandListType type);

        ~DX12CommandQueue() { this->!DX12CommandQueue(); }
        !DX12CommandQueue();

        // native properties
        property ID3D12CommandQueue* NativeQueue
        {
            ID3D12CommandQueue* get() { return m_commandQueue; }
        }
        // other properties
        property DX12CommandListType Type
        {
            DX12CommandListType get() { return m_type; }
        }
        // other properties
        property DX12Fence^ Fence
        {
            DX12Fence^ get() { return m_fence; }
            void set(DX12Fence^ value) { m_fence = value; }
        }

        void ExecuteList(DX12CommandList^ commandList);
        void Wait();
        void WaitForCompletion();
        void Signal();
    };
}
