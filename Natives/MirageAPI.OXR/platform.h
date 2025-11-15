#pragma once

#ifdef UseDX12

namespace MirageAPI::OpenXR::Platform
{
    struct SessionRequirements
    {
        ID3D12Device* device;
        ID3D12CommandQueue* commandQueue;

        const void* GetBindings() const
        {
            XrGraphicsBindingD3D12KHR graphicsBinding = {XR_TYPE_GRAPHICS_BINDING_D3D12_KHR};
            graphicsBinding.device = device;
            graphicsBinding.queue = commandQueue;
            return &graphicsBinding;
        }
    };
}

#endif

#ifdef UseDX11


namespace MirageAPI::OpenXR::Platform
{
    struct SessionRequirements
    {
        ID3D11Device* device;

        const void* GetBindings()
        {
            XrGraphicsBindingD3D11KHR graphicsBinding = {XR_TYPE_GRAPHICS_BINDING_D3D11_KHR};
            graphicsBinding.device = device;
            return &graphicsBinding;
        }
    };
}

#endif

#ifdef UseOpenGL

namespace MirageAPI::OpenXR::Platform
{
    struct SessionRequirements
    {
        XrVersion minApiVersion;
        XrVersion maxApiVersion;

        // const void* GetBindings()
        // {
        //     // TODO: выяснить нужен ли вообще этот метод
        //     return nullptr;
        // }
    };
}

#endif
