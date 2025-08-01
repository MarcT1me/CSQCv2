#include "pch.h"
#include "DX12Fence.h"

namespace MirageAPI::DirectX::Command
{
    DX12Fence::DX12Fence(UINT64 initValue)
    {
        ID3D12Fence* fence;
        CheckHResult(
            device->CreateFence(
                initValue,
                D3D12_FENCE_FLAG_NONE,
                IID_PPV_ARGS(&fence)
            ),
            "Create Fence failed"
        );
        m_fence = fence;

        // creating fence event
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!m_fenceEvent)
            throw gcnew System::Exception("Failed to create fence event");
    }

    void DX12Fence::!DX12Fence()
    {
        Validate();

        if (m_fenceEvent && CloseHandle(m_fenceEvent))
            m_fenceEvent = nullptr;

        SimpleRelease(m_fence);
    }

    HANDLE DX12Fence::OnCompletion()
    {
        m_fence->SetEventOnCompletion(1, m_fenceEvent);
        return m_fenceEvent;
    }

    void DX12Fence::Wait()
    {
        WaitForCompletion();
        IncreaseValue();
    }

    void DX12Fence::WaitForCompletion()
    {
        // wait for signal if needed
        if (CompletedValue < m_fenceValue)
        {
            CheckHResult(
                m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent),
                "Event on completion err"
            );
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }
    }

    void DX12Fence::IncreaseValue()
    {
        m_fenceValue++;
    }
}
