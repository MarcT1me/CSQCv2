#pragma once

// Mirage ecosystem
#include "../DX12ShaderResource.h"
#include "../Buffer/DX12Buffer.h"


// texture
namespace MirageAPI::DirectX::Resource
{
    public ref class DX12Texture : public DX12ShaderResource
    {
    internal:
        // constructors and deconstructors
        DX12Texture(DX12ResourceConfig^ config, nullptr_t);

    public:
        // constructors and deconstructors
        DX12Texture(DX12ResourceConfig^ config);

        // texture operations
        virtual void UploadData(array<Byte>^ data);
        D3D12_SHADER_RESOURCE_VIEW_DESC CreateResourceDesc() override;
    };
}
