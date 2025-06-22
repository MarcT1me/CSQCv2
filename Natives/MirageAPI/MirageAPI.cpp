#include "pch.h"
#include "MirageAPI.h"

#include <GLFW/glfw3.h>

#include "Vulkan/VulkanContext.h"

namespace MirageAPI
{
    void MirageSystem::Initialize(bool initVulkan)
    {
        if (!glfwInit())
        {
            throw gcnew System::Exception("GLFW not initialized");
        }
        
        if (!glfwVulkanSupported())
        {
            throw gcnew System::Exception("Vulkan not supported");
        }
        
        if (initVulkan) {
            InitVulkan();
        }
        
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    void MirageSystem::Deinitialize()
    {
        DeinitializeVulkan();
        glfwTerminate();
    }
    
    void MirageSystem::InitVulkan()
    {
        if (s_vulkanContext == nullptr)
        {
            s_vulkanContext = gcnew Vulkan::VulkanContext();
        }
    }
    
    void MirageSystem::DeinitializeVulkan()
    {
        if (s_vulkanContext != nullptr)
        {
            delete s_vulkanContext;
            s_vulkanContext = nullptr;
        }
    }
}
