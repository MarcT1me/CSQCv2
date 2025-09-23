#include "pch.h"
#include "DX12CommandQueue.h"

namespace MirageAPI::DirectX::Command
{
    DX12CommandQueue::DX12CommandQueue(
        QIdentifier^ identifier,
        DX12CommandListType type,
        DX12Fence^ fence
    ) : DX12Object(gcnew DX12ObjectData(identifier)),
        _type(type),
        _fence(fence)
    {
        // command queue
        D3D12_COMMAND_QUEUE_DESC queueDesc;
        queueDesc.Type = static_cast<D3D12_COMMAND_LIST_TYPE>(_type);
        queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;

        ID3D12CommandQueue* commandQueue;
        CheckHResult(
            device->CreateCommandQueue(
                &queueDesc,
                IID_PPV_ARGS(&commandQueue)
            ),
            "Failed to create command queue."
        );
        _commandQueue = commandQueue;
    }

    void DX12CommandQueue::!DX12CommandQueue()
    {
        Validate();

        SimpleRelease(_commandQueue);
    }

    void DX12CommandQueue::ExecuteList(DX12CommandList^ commandList)
    {
        Validate();

        ID3D12CommandList* ppCommandLists[] = {commandList->NativeList};
        _commandQueue->ExecuteCommandLists(1, ppCommandLists);
    }

    void DX12CommandQueue::Wait()
    {
        WaitForCompletion();
        Signal();
    }


    void DX12CommandQueue::WaitForCompletion()
    {
        Validate();

        // waiting for ends of all operations
        if (_commandQueue->Signal(_fence->NativeFence, 1) && _fence->CompletedValue < 1)
        {
            WaitForSingleObject(_fence->OnCompletion(), INFINITE);
        }
    }

    void DX12CommandQueue::Signal()
    {
        // signal queue
        CheckHResult(
            _commandQueue->Signal(_fence->NativeFence, _fence->Value + 1),
            "Native Queue signal err"
        );
        _fence->IncreaseValue();
    }
}
