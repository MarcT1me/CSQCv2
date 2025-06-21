#pragma once

namespace MirageAPI
{
    namespace Vulkan
    {
        ref class VulkanContext;
    }

    public ref class MirageSystem
    {
        static Vulkan::VulkanContext^ s_vulkanContext = nullptr; // Статический контекст Vulkan
        static bool initVulkan;

    public:
        static void Initialize(bool initVulkan);
        static void Deinitialize();

        static void InitVulkan();
        static void DeinitializeVulkan();
        
        static property Vulkan::VulkanContext^ CurrentContext
        {
            Vulkan::VulkanContext^ get() { return s_vulkanContext; }
        }
    };
}
