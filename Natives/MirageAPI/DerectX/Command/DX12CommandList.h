#pragma once

// Mirage ecosystem
#include "enums.h"
#include "../Resource/Array/DX12IndexBuffer.h"
#include "../Resource/Array/DX12VertexBuffer.h"
#include "../Resource/Array/DX12ConstantBuffer.h"
#include "../Resource/Array/DX12StructuredBuffer.h"

namespace MirageAPI::DirectX
{
    ref class DX12DescriptorHeap;
}

namespace MirageAPI::DirectX::Pipeline
{
    ref class DX12PipelineState;
}

namespace MirageAPI::DirectX::Resource
{
    ref class DX12FrameBuffer;
    ref class DX12Texture;
}

// command list
namespace MirageAPI::DirectX::Command
{
    public ref class DX12CommandList : public DX12Object
    {
    protected:
        // native
        ID3D12CommandAllocator* m_commandAllocator = nullptr;
        ID3D12GraphicsCommandList* m_commandList = nullptr;

    internal:
        // descriptions
        DX12CommandListType m_type;

        DX12DescriptorHeap^ m_descriptorHeap = nullptr; // heap
        Pipeline::DX12PipelineState^ m_pipelineState = nullptr; // pipeline

        void UpdatePipelineState();
        void UpdateDescriptorHeap();

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
        // other properties
        property DX12CommandListType Type
        {
            DX12CommandListType get() { return m_type; }
        }
        property Pipeline::DX12PipelineState^ PipelineState
        {
            Pipeline::DX12PipelineState^ get() { return m_pipelineState; }
            void set(Pipeline::DX12PipelineState^ value) { m_pipelineState = value; }
        }
        property DX12DescriptorHeap^ DescriptorHeap
        {
            DX12DescriptorHeap^ get() { return m_descriptorHeap; }
            void set(DX12DescriptorHeap^ value) { m_descriptorHeap = value; }
        }

        // command list operations
        virtual void Reset();
        virtual void Close();

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
            Resource::DX12FrameBuffer^ frameBuffer,
            float r, float g, float b, float a
        );

        // bindings
        void BindBuffer(Resource::DX12FrameBuffer^ frameBuffer);
        void BindBuffer(Resource::DX12IndexBuffer^ indexBuffer);
        void BindBuffer(Resource::DX12VertexBuffer^ vertexBuffer);

        void BindBuffer(UINT rootIndex, Resource::DX12ConstantBuffer^ constantBuffer);
        void BindBuffer(UINT rootIndex, Resource::DX12StructuredBuffer^ structuredBuffer);

        void SetRootConstants(UINT rootIndex, UINT constantSize, float data[], UINT offset);

        // other
        void ValidateMembers();
    };
}
