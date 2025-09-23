#pragma once

#include "DX12CommandList.h"
#include "DX12Fence.h"

namespace MirageAPI::DirectX::Command
{
    public ref class DX12CommandQueue : public DX12Object<DX12ObjectData^>
    {
        ID3D12CommandQueue* _commandQueue;
        DX12CommandListType _type;
        DX12Fence^ _fence;

    public:
        DX12CommandQueue(QIdentifier^ identifier, DX12CommandListType type, DX12Fence^ fence);

        ~DX12CommandQueue() { this->!DX12CommandQueue(); }
        !DX12CommandQueue();

        // native properties
        property ID3D12CommandQueue* NativeQueue
        {
            ID3D12CommandQueue* get() { return _commandQueue; }
        }
        // other properties
        property DX12CommandListType Type
        {
            DX12CommandListType get() { return _type; }
        }
        // other properties
        property DX12Fence^ Fence
        {
            DX12Fence^ get() { return _fence; }
        }

        void ExecuteList(DX12CommandList^ commandList);
        void Wait();
        void WaitForCompletion();
        void Signal();
    };
}
