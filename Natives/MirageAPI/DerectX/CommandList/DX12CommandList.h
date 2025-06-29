#pragma once

#include "..\DX12Enums.h"
#include "..\DX12PipelineState.h"
#include "..\Buffer\DX12VertexBuffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12CommandList
    {
    protected:
        ID3D12GraphicsCommandList* m_commandList = nullptr;
        ID3D12CommandAllocator* m_commandAllocator = nullptr;
        DX12CommandListType m_type;

    public:
        DX12CommandList(DX12CommandListType type);
        virtual ~DX12CommandList() { this->!DX12CommandList(); }
        !DX12CommandList();

        virtual void Reset();
        virtual void Close();
        void Execute();
        static void WaitForCompletion();

        property ID3D12GraphicsCommandList* NativeCommandList
        {
            ID3D12GraphicsCommandList* get() { return m_commandList; }
        }

        property ID3D12CommandAllocator* NativeAllocator
        {
            ID3D12CommandAllocator* get() { return m_commandAllocator; }
        }

        void SetPipelineState(DX12PipelineState^ pipelineState);
        void SetGraphicsRootSignature(DX12PipelineState^ pipelineState);
        void IASetPrimitiveTopology(PrimitiveTopology topology);
        void IASetVertexBuffer(DX12VertexBuffer^ vertexBuffer);
        void DrawInstanced(UINT vertexCount, UINT instanceCount, UINT startVertex, UINT startInstance);

        void SetViewport(
            float left, float top,
            float width, float height,
            float minDepth, float maxDepth
        );
        void SetScissorRect(
            int left, int top,
            int right, int bottom
        );
    };
}
