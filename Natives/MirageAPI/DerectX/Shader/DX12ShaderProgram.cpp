#include "pch.h"
#include "DX12ShaderProgram.h"

#include "../Descriptors/DX12DescriptorHeap.h"
#include "../Resource/DX12ShaderResource.h"

namespace MirageAPI::DirectX::Shader
{
    DX12ShaderProgram::DX12ShaderProgram(
        QIdentifier^ identifier,
        array<DX12Shader^>^ shaders,
        array<Pipeline::DX12InputElement^>^ inputs,
        array<Pipeline::DX12RootParameter^>^ resources
    ) : DX12Object(gcnew DX12ObjectData(identifier)),
        _shaders(shaders),
        _pipelineConfig(gcnew Pipeline::DX12PipelineStateConfig(
            identifier, resources, nullptr, gcnew Pipeline::DX12RasterizerConfig(), gcnew Pipeline::DX12BlendConfig(true, false), inputs
        ))
    {
    }

    void DX12ShaderProgram::!DX12ShaderProgram()
    {
        SimpleDelete(_pipelineState)
        SimpleDelete(_descriptorHeap)
    }

    void DX12ShaderProgram::BindResource(Resource::DX12Resource^ resource)
    {
        // check type
        switch (resource->MetaData->Type)
        {
        case DX12ResourceType::Texture:
        case DX12ResourceType::StructuredBuffer:
            break;
        default:
            throw gcnew DXException("Invalid resource type. Can Bind only Texture, StructuredBuffer or ConstantBuffer.");
        }

        // saving in list
        if (!_boundResources->TryAdd(resource->Id, resource))
        {
            throw gcnew DXException("Adding error. Maybe resource already bound");
        }
    }

    void DX12ShaderProgram::ReleaseResource(QIdentifier^ identifier)
    {
        // remove from list
        Resource::DX12Resource^ resource;
        if (!_boundResources->TryRemove(identifier, resource))
        {
            throw gcnew DXException("Resource '" + identifier + "' does not exist");
        }

        // release resource from GRAM
        switch (resource->MetaData->Type)
        {
        case DX12ResourceType::Texture:
        case DX12ResourceType::StructuredBuffer:
            safe_cast<Resource::DX12ShaderResource^>(resource)->ReleaseResource(_descriptorHeap);
        default: break;
        }
    }

    void DX12ShaderProgram::Prepare()
    {
        // creating SamplerConfig if it is not configured
        if (_pipelineConfig->SamplerConfigs == nullptr)
        {
            bool hasTextures = false;
            for each (auto res in _pipelineConfig->RootParams)
            {
                if (res->Type == DX12ResourceType::Texture)
                {
                    hasTextures = true;
                    break;
                }
            }

            _pipelineConfig->SamplerConfigs = gcnew array<Pipeline::DX12SamplerConfig^>(hasTextures);

            // if texture exist in resources and SamplerConfig not set - creating default sampler
            if (hasTextures)
                _pipelineConfig->SamplerConfigs[0] = gcnew Pipeline::DX12SamplerConfig(0, DX12ShaderVisibility::All);
        }

        // creating final pipline state
        _pipelineState = gcnew Pipeline::DX12PipelineState(_shaders, _pipelineConfig);

        // create descriptor heap by using bound Resources
        _descriptorHeap = gcnew Descriptors::DX12DescriptorHeap(gcnew Descriptors::DX12DescriptorHeapData(
            Id, Descriptors::DX12DescriptorHeapType::SRV, _boundResources->Count, true, false
        ));
        // allocating resources in GRAM
        for each (auto resource in _boundResources->Values)
        {
            switch (resource->MetaData->Type)
            {
            case DX12ResourceType::Texture:
            case DX12ResourceType::StructuredBuffer:
                {
                    if (!safe_cast<Resource::DX12ShaderResource^>(resource)->AllocateResource(_descriptorHeap))
                    {
                        throw gcnew DXException("Resource " + resource->Id + " allocation error");
                    }
                }
            default: break;
            }
        }
    }

    void DX12ShaderProgram::BeginFrame(Descriptors::DX12Descriptor^ targetDescriptor, Command::DX12CommandList^ commandList)
    {
        _renderTarget = targetDescriptor;
        _commandList = commandList;

        // apply state
        _commandList->Reset();
        _commandList->SetPipelineState(_pipelineState);
        _commandList->SetDescriptorHeap(_descriptorHeap);

        // set frame buffer
        _commandList->SetRenderTarget(targetDescriptor);
    }

    void DX12ShaderProgram::Clear(Color4 color)
    {
        _commandList->ClearTarget(color.R, color.G, color.B, color.A);
    }

    // void DX12ShaderProgram::Render(
    //     Command::DX12PrimitiveTopology topology,
    //     Resource::GeometryBuffer^ geometry
    // )
    // {
    //     Bind(geometry);
    //     Render(topology, geometry->VertexCount, 1, 0, 0);
    // }

    // void DX12ShaderProgram::Render(
    //     Command::DX12PrimitiveTopology topology,
    //     Resource::GeometryBuffer^ geometry,
    //     UINT startVertex
    // )
    // {
    //     Bind(geometry);
    //     Render(topology, geometry->VertexCount, 1, startVertex, 0);
    // }

    void DX12ShaderProgram::Render(
        Command::DX12PrimitiveTopology topology,
        Resource::DX12Buffer^ vertexBuffer, Resource::DX12Buffer^ indexBuffer,
        UINT vertexCount
    )
    {
        Render(topology, indexBuffer, vertexBuffer, vertexCount, 0);
    }

    void DX12ShaderProgram::Render(
        Command::DX12PrimitiveTopology topology,
        Resource::DX12Buffer^ vertexBuffer, Resource::DX12Buffer^ indexBuffer,
        UINT vertexCount,
        UINT startVertex
    )
    {
        Bind(vertexBuffer, indexBuffer);
        Render(topology, vertexCount, 1, startVertex, 0);
    }

    void DX12ShaderProgram::Render(
        Command::DX12PrimitiveTopology topology,
        UINT vertexCount, UINT instanceCount
    )
    {
        Render(topology, vertexCount, instanceCount, 0, 0);
    }

    void DX12ShaderProgram::Render(
        Command::DX12PrimitiveTopology topology,
        UINT vertexCount, UINT instanceCount,
        UINT startVertex, UINT startInstance
    )
    {
        _commandList->SetPrimitiveTopology(topology);
        _commandList->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);
    }

    // void DX12ShaderProgram::Bind(Resource::GeometryBuffer^ geometry)
    // {
    //     Bind(geometry->VertexBuffer, geometry->IndexBuffer);
    // }

    void DX12ShaderProgram::Bind(Resource::DX12Buffer^ vertexBuffer, Resource::DX12Buffer^ indexBuffer)
    {
        _commandList->SetVertexBuffer(vertexBuffer);
        CheckNull(indexBuffer) return;
        _commandList->SetIndexBuffer(vertexBuffer);
    }

    void DX12ShaderProgram::Bind(UINT index, Resource::DX12Buffer^ constantBuffer)
    {
        _commandList->BindConstantBuffer(index, constantBuffer);
    }

    void DX12ShaderProgram::Bind(UINT index, Resource::DX12ShaderResource^ resource)
    {
        _commandList->BindResource(index, resource);
    }

    void DX12ShaderProgram::Bind(UINT index, Descriptors::DX12Descriptor^ descriptor)
    {
        _commandList->BindDescriptor(index, descriptor);
    }

    void DX12ShaderProgram::SetConstants(UINT rootIndex, UINT constantSize, array<float>^ data, UINT offset)
    {
        _commandList->SetRootConstants(rootIndex, constantSize, data, offset);
    }

    void DX12ShaderProgram::SetConstant(UINT rootIndex, UINT data, UINT offset)
    {
        _commandList->SetRootConstant(rootIndex, data, offset);
    }

    void DX12ShaderProgram::ReleaseFrame()
    {
        _commandList->Close(); // closing command list for adding new commands

        _renderTarget = nullptr;
        _commandList = nullptr;
    }
}
