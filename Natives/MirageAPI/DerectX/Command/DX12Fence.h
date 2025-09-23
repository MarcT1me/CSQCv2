#pragma once

#include "../DX12ObjectData.h"

namespace MirageAPI::DirectX::Command
{
    public ref class DX12Fence : public DX12Object<DX12ObjectData^>
    {
        ID3D12Fence* m_fence = nullptr;
        HANDLE m_fenceEvent = nullptr;
        UINT64 m_fenceValue = 1;

    public:
        DX12Fence(QIdentifier^ identifier, UINT64 initValue);

        ~DX12Fence() { this->!DX12Fence(); }
        !DX12Fence();
        
        property ID3D12Fence* NativeFence
        {
            ID3D12Fence* get() { return m_fence; }
        }
        property HANDLE NativeEvent
        {
            HANDLE get() { return m_fenceEvent; }
        }
        property UINT64 Value
        {
            UINT64 get() { return m_fenceValue; }
        }
        property UINT64 CompletedValue
        {
            UINT64 get() { return m_fence->GetCompletedValue(); }
        }
        
        HANDLE OnCompletion();
        void Wait();
        void WaitForCompletion();
        void IncreaseValue();
    };
}
