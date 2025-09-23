#pragma once

#include "../Pipeline/DX12PipelineState.h"
#include "../Command/DX12CommandList.h"
#include "../Resource/DX12Resource.h"

namespace MirageAPI::DirectX::Shader
{
    public ref class DX12ShaderProgram : DX12Object<DX12ObjectData^>, QuantumCore::Objects::IPreparable
    {
        // cogs
        array<DX12Shader^>^ _shaders;
        Pipeline::DX12PipelineStateConfig^ _pipelineConfig;
        Pipeline::DX12PipelineState^ _pipelineState;
        Descriptors::DX12DescriptorHeap^ _descriptorHeap;
        // resources
        Command::DX12CommandList^ _commandList;
        Descriptors::DX12Descriptor^ _renderTarget;
        QuantumCore::Data::Collections::ConcurrentIdentifierMap<Resource::DX12Resource^>^ _boundResources =
            gcnew QuantumCore::Data::Collections::ConcurrentIdentifierMap<Resource::DX12Resource^>(nullptr);

    public:
        Rect^ Viewport;
        int Near;
        int Far;

        DX12ShaderProgram(
            QIdentifier^ identifier,
            array<DX12Shader^>^ shaders,
            array<Pipeline::DX12InputElement^>^ inputs,
            array<Pipeline::DX12RootParameter^>^ resources
        );

        ~DX12ShaderProgram() { this->!DX12ShaderProgram(); }
        !DX12ShaderProgram();

        property Descriptors::DX12DescriptorHeap^ DescriptorHeap
        {
            Descriptors::DX12DescriptorHeap^ get() { return _descriptorHeap; }
        }
        property Pipeline::DX12PipelineStateConfig^ PipelineConfig
        {
            Pipeline::DX12PipelineStateConfig^ get() { return _pipelineConfig; }
        }

        void BindResource(Resource::DX12Resource^ resource);
        void ReleaseResource(QIdentifier^ identifier);

        virtual void Prepare();

        // render
        void BeginFrame(Descriptors::DX12Descriptor^ targetDescriptor, Command::DX12CommandList^ commandList);

        void Clear(Color4 color);

        // extended render methods // TODO: GeometryBuffer class
        // void Render(
        //     Command::DX12PrimitiveTopology topology,
        //     Resource::GeometryBuffer^ geometry
        // );
        // void Render(
        //     Command::DX12PrimitiveTopology topology,
        //     Resource::GeometryBuffer^ geometry,
        //     UINT startVertex
        // );
        void Render(
            Command::DX12PrimitiveTopology topology,
            Resource::DX12Buffer^ vertexBuffer, Resource::DX12Buffer^ indexBuffer,
            UINT vertexCount
        );
        void Render(
            Command::DX12PrimitiveTopology topology,
            Resource::DX12Buffer^ vertexBuffer, Resource::DX12Buffer^ indexBuffer,
            UINT vertexCount,
            UINT startVertex
        );
        // simple render methods
        void Render(
            Command::DX12PrimitiveTopology topology,
            UINT vertexCount, UINT instanceCount
        );
        void Render(
            Command::DX12PrimitiveTopology topology,
            UINT vertexCount, UINT instanceCount,
            UINT startVertex, UINT startInstance
        );

        // Geometry
        // void Bind(Resource::GeometryBuffer^ geometry); // TODO: GeometryBuffer class
        void Bind(Resource::DX12Buffer^ vertexBuffer, Resource::DX12Buffer^ indexBuffer);

        // resources
        void Bind(UINT index, Resource::DX12Buffer^ constantBuffer);
        void Bind(UINT index, Resource::DX12ShaderResource^ resource);
        void Bind(UINT index, Descriptors::DX12Descriptor^ descriptor);

        // root constants
        void SetConstants(UINT index, UINT constantSize, array<float>^ data, UINT offset);
        void SetConstant(UINT index, UINT data, UINT offset);

        void ReleaseFrame();
    };
}
