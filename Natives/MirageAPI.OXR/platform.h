#pragma once

#ifdef UseDX12

namespace MirageAPI::Native::OpenXR::Platform
{
    struct SessionRequirements
    {
        XrGraphicsBindingD3D12KHR Bindings;

        SessionRequirements(ID3D12Device* device, ID3D12CommandQueue* queue)
            : Bindings(XrGraphicsBindingD3D12KHR{XR_TYPE_GRAPHICS_BINDING_D3D12_KHR, nullptr, device, queue})
        {
        }

        const void* GetBindings()
        {
            return &Bindings;
        }
    };
}

#endif

#ifdef UseDX11

namespace MirageAPI::Native::OpenXR::Platform
{
    struct SessionRequirements
    {
        XrGraphicsBindingD3D11KHR Bindings;

        SessionRequirements(ID3D12Device* device)
            : Bindings(XrGraphicsBindingD3D12KHR{XR_TYPE_GRAPHICS_BINDING_D3D11_KHR, nullptr, device})
        {
        }

        const void* GetBindings()
        {
            return &Bindings;
        }
    };
}

#endif

#ifdef UseOpenGL

namespace MirageAPI::Native::OpenXR::Platform
{
    struct SessionRequirements
    {
    };
}

#endif

#ifdef UseVulkan

namespace MirageAPI::Native::OpenXR::Platform
{
    struct SessionRequirements
    {
    };
}

#endif
