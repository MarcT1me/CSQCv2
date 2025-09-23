#pragma once

// Mirage ecosystem
#include "enums.h"
#include "../Descriptors/DX12Descriptor.h"

namespace MirageAPI::DirectX::Command
{
    ref class DX12CommandQueue;
}

namespace MirageAPI::DirectX::Descriptors
{
    ref class DX12DescriptorHeap;
}

namespace MirageAPI::DirectX::Pipeline
{
    ref class DX12PipelineState;
}

namespace MirageAPI::DirectX::Resource
{
    ref class DX12Buffer;
    ref class DX12RenderTarget;
    ref class DX12Texture;
}

// command list
namespace MirageAPI::DirectX::Command
{
    public ref class DX12CommandList : public DX12Object<DX12ObjectData^>
    {
        Descriptors::DX12Descriptor^ _targetDescriptor;

    protected:
        // native
        ID3D12CommandAllocator* _commandAllocator = nullptr;
        ID3D12GraphicsCommandList* _commandList = nullptr;
        // descriptions
        DX12CommandListType _type;

    public:
        // constructors and deconstructors
        DX12CommandList(
            QIdentifier^ identifier,
            DX12CommandQueue^ queue
        );

        ~DX12CommandList() { this->!DX12CommandList(); }
        !DX12CommandList();

        // native properties
        property ID3D12CommandAllocator* NativeAllocator
        {
            ID3D12CommandAllocator* get() { return _commandAllocator; }
        }
        property ID3D12GraphicsCommandList* NativeList
        {
            ID3D12GraphicsCommandList* get() { return _commandList; }
        }
        // other properties
        property DX12CommandListType Type
        {
            DX12CommandListType get() { return _type; }
        }

        // command list operations
        virtual void Reset();
        virtual void Close();

        void SetPipelineState(Pipeline::DX12PipelineState^ pipelineState);
        void SetDescriptorHeap(Descriptors::DX12DescriptorHeap^ descriptorHeap);
        void SetDescriptorHeaps(array<Descriptors::DX12DescriptorHeap^>^ descriptorHeap);

        // Render field
        void SetViewport(
            float left, float top,
            float width, float height,
            float minDepth, float maxDepth
        );
        void SetScissorRect(
            int left, int top,
            int right, int bottom
        );

        // render methods
        void SetPrimitiveTopology(DX12PrimitiveTopology topology);
        void DrawInstanced(UINT vertexCount, UINT count, UINT startVertex, UINT startInstance);
        void ClearTarget(Color4 color);
        void ClearTarget(float r, float g, float b, float a);

        // Render Target
        void SetRenderTarget(Descriptors::DX12Descriptor^ targetDescriptor);

        // geometr
        void SetIndexBuffer(Resource::DX12Buffer^ buffer);
        void SetVertexBuffer(Resource::DX12Buffer^ buffer);

        // resources
        void BindConstantBuffer(UINT index, Resource::DX12Buffer^ resource);
        void BindResource(UINT index, Resource::DX12ShaderResource^ resource);
        void BindDescriptor(UINT index, Descriptors::DX12Descriptor^ descriptor);

        // root constants
        void SetRootConstants(UINT index, UINT constantSize, array<float>^ data, UINT offset);
        void SetRootConstant(UINT index, UINT data, UINT offset);
    };
}
