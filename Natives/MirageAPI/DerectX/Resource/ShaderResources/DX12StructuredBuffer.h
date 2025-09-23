#pragma once

// Mirage ecosystem
#include "../Buffer/DX12Buffer.h"
#include "../DX12ShaderResource.h"

namespace MirageAPI::DirectX::Descriptors
{
    ref class DX12DescriptorHeap;
}

// buffer
namespace MirageAPI::DirectX::Resource
{
    public ref class DX12StructuredBuffer : public DX12ShaderResource
    {
    protected:
        D3D12_SHADER_RESOURCE_VIEW_DESC CreateResourceDesc() override;

    public:
        // constructors and deconstructors
        DX12StructuredBuffer(
            DX12ResourceConfig^ config
        ): DX12ShaderResource(config)
        {
        }
    };
}
