#pragma once

#include "DX12CommandList.h"

namespace MirageAPI::DirectX
{
    public ref class DX12WindowCommandList sealed : public DX12CommandList
    {
    internal:
        DX12WindowCommandList(
            ID3D12GraphicsCommandList* commandList,
            ID3D12CommandAllocator* commandAllocator
        ) : DX12CommandList(D3D12_COMMAND_LIST_TYPE_DIRECT)
        {
            if (m_commandList) m_commandList->Release();
            if (m_commandAllocator) m_commandAllocator->Release();
            
            m_commandList = commandList;
            m_commandAllocator = commandAllocator;
        }
        
    public:
        void Reset() override {}
        void Close() override {}
    };
}