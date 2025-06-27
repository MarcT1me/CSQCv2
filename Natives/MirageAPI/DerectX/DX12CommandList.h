#pragma once

#include "DX12Enums.h"
#include "DX12PipelineState.h"
#include "DX12VertexBuffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12CommandList
    {
    internal:
        ID3D12GraphicsCommandList* m_commandList = nullptr;
        ID3D12CommandAllocator* m_commandAllocator = nullptr;
        D3D12_COMMAND_LIST_TYPE m_type;

    public:
        DX12CommandList(D3D12_COMMAND_LIST_TYPE type);
        ~DX12CommandList();
        !DX12CommandList();

        virtual void Reset();
        virtual void Close();
        void Execute();
        static void WaitForCompletion();

        property ID3D12GraphicsCommandList* NativeCommandList { 
            ID3D12GraphicsCommandList* get() { return m_commandList; } 
        }
        
        property ID3D12CommandAllocator* NativeAllocator { 
            ID3D12CommandAllocator* get() { return m_commandAllocator; } 
        }

        void SetPipelineState(DX12PipelineState^ pipelineState);
        void SetGraphicsRootSignature(DX12PipelineState^ pipelineState);
        void IASetPrimitiveTopology(PrimitiveTopology topology);
        void IASetVertexBuffer(DX12VertexBuffer^ vertexBuffer);
        void DrawInstanced(UINT vertexCount, UINT instanceCount, UINT startVertex, UINT startInstance);
    };
}
