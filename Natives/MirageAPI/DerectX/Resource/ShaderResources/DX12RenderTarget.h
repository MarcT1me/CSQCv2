#pragma once

#include "DX12Texture.h"

namespace MirageAPI::DirectX::Descriptors
{
    ref class DX12DescriptorHeap;
}

namespace MirageAPI::DirectX::Resource
{
    public ref class DX12RenderTarget : public DX12Texture
    {
    internal:
        // constructors and deconstructors
        DX12RenderTarget(DX12ResourceConfig^ config, ID3D12Resource* resource);

        void CreateDeviceView(Descriptors::DX12Descriptor^ descriptor) override;

    public:
        DX12RenderTarget(DX12ResourceConfig^ config);

        String^ ToString() override;
    };
}
