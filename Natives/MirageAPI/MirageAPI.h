#pragma once

namespace MirageAPI
{
    namespace Vulkan
    {
        ref class VulkanContext;
    }

    namespace OpenGL
    {
        ref class OpenGLContext;
    }

    public ref class MirageSystem
    {
        static Vulkan::VulkanContext^ s_vulkanContext = nullptr;

    public:
        static bool initVulkan;
        static bool initOpenGl;
        
        static void Initialize(bool initVulkan, bool initOpenGl);
        static void Deinitialize();

        static void InitVulkan();
        static void DeinitializeVulkan();

        static property Vulkan::VulkanContext^ CurrentContext
        {
            Vulkan::VulkanContext^ get() { return s_vulkanContext; }
        }
    };
}
