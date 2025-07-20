#pragma once

#include "..\Pipeline\DX12PipelineState.h"
#include "..\Buffer\DX12FrameBuffer.h"
#include "..\Buffer\DX12IndexBuffer.h"
#include "..\Buffer\DX12VertexBuffer.h"
#include "..\Buffer\DX12ConstantBuffer.h"
#include "..\Buffer\DX12StructuredBuffer.h"

namespace MirageAPI::DirectX
{
    ref class DX12Texture;
    ref class DX12DescriptorHeap;

    public ref class DX12CommandList
    {
    internal:
        ID3D12CommandAllocator* m_commandAllocator = nullptr;
        ID3D12GraphicsCommandList* m_commandList = nullptr;
        ID3D12CommandQueue* m_commandQueue;
        DX12CommandListType m_type;
        DX12DescriptorHeap^ m_descriptorHeap;

    public:
        DX12CommandList(DX12CommandListType type);
        virtual ~DX12CommandList() { this->!DX12CommandList(); }
        !DX12CommandList();

        virtual void Reset();
        virtual void Close();
        void Execute();
        void WaitForCompletion();

        property ID3D12CommandAllocator* NativeAllocator
        {
            ID3D12CommandAllocator* get() { return m_commandAllocator; }
        }

        property ID3D12GraphicsCommandList* NativeList
        {
            ID3D12GraphicsCommandList* get() { return m_commandList; }
        }

        property ID3D12GraphicsCommandList* NativeQueue
        {
            ID3D12GraphicsCommandList* get() { return m_commandList; }
        }

        void SetPipelineState(DX12PipelineState^ pipelineState)
        {
            m_commandList->SetPipelineState(pipelineState->NativePSO);
            m_commandList->SetGraphicsRootSignature(pipelineState->RootSignature);
        }

        void SetPrimitiveTopology(DX12PrimitiveTopology topology)
        {
            m_commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(topology));
        }

        void DrawInstanced(UINT vertexCount, UINT instanceCount, UINT startVertex, UINT startInstance)
        {
            m_commandList->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);
        }

        void SetDescriptorHeap(DX12DescriptorHeap^ heap);
        void SetTextureSRV(UINT rootIndex, DX12Texture^ texture);
        void TransitionTexture(DX12Texture^ texture, DX12ResourceState newState);
        void SetViewport(
            float left, float top,
            float width, float height,
            float minDepth, float maxDepth
        );
        void SetScissorRect(
            int left, int top,
            int right, int bottom
        );

        void ClearRenderTargetView(DX12FrameBuffer^ frameBuffer, float r, float g, float b, float a);

        void BindBuffer(DX12VertexBuffer^ vertexBuffer)
        {
            vertexBuffer->Bind(m_commandList);
        }

        void BindBuffer(DX12IndexBuffer^ indexBuffer)
        {
            indexBuffer->Bind(m_commandList);
        }

        void BindBuffer(unsigned int rootIndex, DX12ConstantBuffer^ constantBuffer)
        {
            m_commandList->SetGraphicsRootConstantBufferView(
                rootIndex,
                constantBuffer->GPUAddress
            );
        }

        void BindBuffer(unsigned int rootIndex, DX12StructuredBuffer^ structuredBuffer)
        {
            m_commandList->SetGraphicsRootShaderResourceView(
                rootIndex,
                structuredBuffer->GPUAddress
            );
        }

        void SetRootConstants(unsigned int rootIndex, unsigned int constantSize, float data[], unsigned int offset)
        {
            m_commandList->SetGraphicsRoot32BitConstants(
                rootIndex,
                constantSize,
                data,
                offset
            );
        }
    };
}
