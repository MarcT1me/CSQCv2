#include "pch.h"
#include "DX12CommandQueue.h"

namespace MirageAPI::DirectX::Command
{
    DX12CommandQueue::DX12CommandQueue(
        DX12CommandListType type
    ) : m_type(type)
    {
        // command queue
        D3D12_COMMAND_QUEUE_DESC queueDesc;
        queueDesc.Type = static_cast<D3D12_COMMAND_LIST_TYPE>(m_type);
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
        m_commandQueue = commandQueue;
    }

    void DX12CommandQueue::!DX12CommandQueue()
    {
        Validate();

        DXSimpleRelease(m_commandQueue);
    }

    void DX12CommandQueue::ExecuteList(DX12CommandList^ commandList)
    {
        Validate();

        ID3D12CommandList* ppCommandLists[] = {commandList->NativeList};
        m_commandQueue->ExecuteCommandLists(1, ppCommandLists);
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
        if (m_commandQueue->Signal(m_fence->NativeFence, 1) && m_fence->CompletedValue < 1)
        {
            WaitForSingleObject(m_fence->OnCompletion(), INFINITE);
        }
    }

    void DX12CommandQueue::Signal()
    {
        // signal queue
        CheckHResult(
            m_commandQueue->Signal(m_fence->NativeFence, m_fence->Value + 1),
            "Native Queue signal err"
        );
        m_fence->IncreaseValue();
    }
}
