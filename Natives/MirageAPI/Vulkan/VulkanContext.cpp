#include "pch.h"
#include "VulkanContext.h"

#include <set>
#include <GLFW/glfw3.h>
#include <vector>
#include <stdexcept>
#include <msclr/marshal.h>

namespace MirageAPI::Vulkan
{
    VulkanContext::VulkanContext(System::String^ appName, int appVersion[3])
    {
        deviceProperties = new VkPhysicalDeviceProperties();
        std::string nativeAppName = msclr::interop::marshal_as<std::string>(appName);
        Initialize(nativeAppName, appVersion);
    }

    VulkanContext::~VulkanContext()
    {
        delete deviceProperties;
        Cleanup();
    }

    VulkanContext::!VulkanContext()
    {
        Cleanup();
    }

    void VulkanContext::Initialize(const std::string& nativeAppName, int appVersion[3])
    {
        CreateInstance(nativeAppName.c_str(), appVersion);
        SelectPhysicalDevice();
        CreateLogicalDevice();
    }

    void VulkanContext::Cleanup()
    {
        if (device)
        {
            vkDestroyDevice(device, nullptr);
            device = VK_NULL_HANDLE;
        }

        if (instance)
        {
            vkDestroyInstance(instance, nullptr);
            instance = VK_NULL_HANDLE;
        }
    }

    void VulkanContext::CreateInstance(const char* nativeAppName, int appVersion[3])
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

        appInfo.pApplicationName = nativeAppName;
        appInfo.applicationVersion = VK_MAKE_VERSION(appVersion[0], appVersion[1], appVersion[2]);

        appInfo.pEngineName = "Mirage Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Получаем необходимые расширения
        uint32_t glfwExtensionCount;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        VkInstance inst;
        if (vkCreateInstance(&createInfo, nullptr, &inst) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create Vulkan instance!");
        }
        instance = inst;
    }

    void VulkanContext::SelectPhysicalDevice()
    {
        uint32_t deviceCount;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw gcnew System::Exception("Failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        // Выбираем первое устройство с поддержкой RTX
        for (const auto& device : devices)
        {
            if (CheckRTXSupport(device))
            {
                physicalDevice = device;
                rtxSupported = true;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE)
        {
            physicalDevice = devices[0];
        }

        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        for (uint32_t i = 0; i < queueFamilyCount; i++)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphicsQueueFamilyIndex = i;
                break;
            }
        }
        vkGetPhysicalDeviceProperties(physicalDevice, deviceProperties);
    }

    bool VulkanContext::CheckRTXSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions = {
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME
        };

        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    void VulkanContext::CreateLogicalDevice()
    {
        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        std::vector deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        if (rtxSupported)
        {
            deviceExtensions.push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
            deviceExtensions.push_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
            deviceExtensions.push_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        createInfo.enabledLayerCount = 0;

        VkDevice dev;
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &dev) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create logical device!");
        }
        device = dev;

        VkQueue queue;
        vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &queue);
        graphicsQueue = queue;

        VkQueue pQueue;
        vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &pQueue);
        presentQueue = pQueue;
    }

    uint32_t VulkanContext::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if (typeFilter & 1 << i && 
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
                }
        }

        throw gcnew System::Exception("Failed to find suitable memory type!");
    }
}
