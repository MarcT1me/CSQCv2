#pragma once

// Mirage ecosystem
#include "../DX12Object.h"
#include "../Buffer/DX12IndexBuffer.h"
#include "../Buffer/DX12VertexBuffer.h"
#include "../Buffer/DX12ConstantBuffer.h"
#include "../Buffer/DX12StructuredBuffer.h"

namespace MirageAPI::DirectX
{
    ref class DX12PipelineState;
    ref class DX12DescriptorHeap;
    ref class DX12FrameBuffer;
    ref class DX12Texture;
}

// command list
namespace MirageAPI::DirectX
{
    public ref class DX12CommandList : public DX12Object
    {
    protected:
        // native
        ID3D12CommandAllocator* m_commandAllocator = nullptr;
        ID3D12GraphicsCommandList* m_commandList = nullptr;
        ID3D12CommandQueue* m_commandQueue = nullptr;

    internal:
        // descriptions
        DX12CommandListType m_type;

        DX12DescriptorHeap^ m_descriptorHeap = nullptr; // heap
        DX12PipelineState^ m_pipelineState = nullptr; // pipeline

    public:
        // constructors and deconstructors
        DX12CommandList(
            DX12CommandListType type
        );

        ~DX12CommandList() { this->!DX12CommandList(); }
        !DX12CommandList();

        // native properties
        property ID3D12CommandAllocator* NativeAllocator
        {
            ID3D12CommandAllocator* get() { return m_commandAllocator; }
        }
        property ID3D12GraphicsCommandList* NativeList
        {
            ID3D12GraphicsCommandList* get() { return m_commandList; }
        }
        property ID3D12CommandQueue* NativeQueue
        {
            ID3D12CommandQueue* get() { return m_commandQueue; }
        }
        // other properties
        property DX12CommandListType Type
        {
            DX12CommandListType get() { return m_type; }
        }
        property DX12DescriptorHeap^ DescriptorHeap
        {
            DX12DescriptorHeap^ get() { return m_descriptorHeap; }
            void set(DX12DescriptorHeap^ value);
        }
        property DX12PipelineState^ PipelineState
        {
            DX12PipelineState^ get() { return m_pipelineState; }
            void set(DX12PipelineState^ value);
        }

        // command list operations
        virtual void Reset();
        virtual void Close();
        void Execute();
        void WaitForCompletion();

        // Viewport and other
        void SetViewport(
            float left, float top,
            float width, float height,
            float minDepth, float maxDepth
        );
        void SetScissorRect(
            int left, int top,
            int right, int bottom
        );

        // render
        void SetPrimitiveTopology(
            DX12PrimitiveTopology topology
        );
        void DrawInstanced(
            UINT vertexCount,
            UINT instanceCount,
            UINT startVertex,
            UINT startInstance
        );
        void ClearRenderTargetView(
            DX12FrameBuffer^ frameBuffer,
            float r, float g, float b, float a
        );

        // bindings
        void SetTexture(UINT rootIndex, DX12Texture^ texture);
        void SetRootConstants(UINT rootIndex, UINT constantSize, float data[], UINT offset);

        void BindBuffer(DX12IndexBuffer^ indexBuffer);
        void BindBuffer(DX12VertexBuffer^ vertexBuffer);
        void BindBuffer(UINT rootIndex, DX12ConstantBuffer^ constantBuffer);
        void BindBuffer(UINT rootIndex, DX12StructuredBuffer^ structuredBuffer);

        // other
        void Validate() override;
    };
}
