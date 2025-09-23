#include "pch.h"
#include "DX12StructuredBuffer.h"

namespace MirageAPI::DirectX::Resource
{
    D3D12_SHADER_RESOURCE_VIEW_DESC DX12StructuredBuffer::CreateResourceDesc()
    {
        auto desc = DX12ShaderResource::CreateResourceDesc();
        desc.Buffer.FirstElement = 0;
        desc.Buffer.NumElements = MetaData->Width;
        desc.Buffer.StructureByteStride = MetaData->Stride;
        desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
        return desc;
    }
}
