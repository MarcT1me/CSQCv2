#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace MirageAPI::Vulkan
{
    public ref class VulkanContext
    {
        VkInstance instance = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties* deviceProperties;

        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;

        bool rtxSupported = false;
        uint32_t graphicsQueueFamilyIndex;

        void CreateInstance(const char* nativeAppName, int appVersion[3]);
        void SelectPhysicalDevice();
        void CreateLogicalDevice();
        static bool CheckRTXSupport(VkPhysicalDevice device);

        void Initialize(const std::string& nativeAppName, int appVersion[3]);
        void Cleanup();

    public:
        VulkanContext(System::String^ appName, int appVersion[3]);
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
        property VkPhysicalDevice PhysicalDevice
        {
            VkPhysicalDevice get() { return physicalDevice; }
        }
        property VkPhysicalDeviceProperties DeviceProperties
        {
            VkPhysicalDeviceProperties get() { return *deviceProperties; }
        }

        property VkQueue GraphicsQueue { VkQueue get() { return graphicsQueue; } }
        property VkQueue PresentQueue { VkQueue get() { return presentQueue; } }

        property bool IsRTXSupported { bool get() { return rtxSupported; } }
        property uint32_t GraphicsQueueFamilyIndex { uint32_t get() { return graphicsQueueFamilyIndex; } }
        
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };
}
