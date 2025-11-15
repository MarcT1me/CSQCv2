#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// OpenXR defines
#define XR_USE_PLATFORM_WIN64

// Graphic Platforms
#ifdef UseDX12
// TODO: create DX12 wrapper (Max priority)
#define XR_USE_GRAPHICS_API_D3D12

#include <d3d12.h>
#endif

#ifdef UseDX11
// TODO: create DX11 wrapper
#define XR_USE_GRAPHICS_API_D3D11

#include <d3d11.h>
#endif

#ifdef UseOpenGL
// TODO: create OGL wrapper (Max priority)
#define XR_USE_GRAPHICS_API_OPENGL
#endif

#ifdef UseVulkan
// TODO: create Vulkan wrapper (Min Priority)
#define XR_USE_GRAPHICS_API_VULKAN
#endif

// includes
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

// other
#include "XRException.h"
#include "Instance/XRInstance.h"

#endif
