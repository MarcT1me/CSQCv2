#pragma once

#include <vulkan/vulkan.h>

namespace MirageAPI::Vulkan
{
    public ref class VulkanContext
    {
        VkInstance instance = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        bool rtxSupported = false;

        void CreateInstance();
        void SelectPhysicalDevice();
        void CreateLogicalDevice();
        bool CheckRTXSupport(VkPhysicalDevice device);

    public:
        VulkanContext();
        ~VulkanContext();
        !VulkanContext();

        property VkInstance Instance
        {
            VkInstance get() { return instance; }
        }
        property VkDevice Device
        {
            VkDevice get() { return device; }
        }
        property bool IsRTXSupported
        {
            bool get() { return rtxSupported; }
        }

        void Initialize();
        void Cleanup();
    };
}
