#include "pch.h"
#include "MirageAPI.h"

#include <GLFW/glfw3.h>

#include "Vulkan/VulkanContext.h"

namespace MirageAPI
{
    void MirageSystem::Initialize(bool initVulkan, bool initOpenGl)
    {
        if (!glfwInit())
        {
            throw gcnew System::Exception("GLFW not initialized");
        }

        if (initVulkan && initOpenGl)
        {
            throw gcnew System::Exception("Cannot initialize both Vulkan and OpenGL at the same time");
        }

        MirageSystem::initVulkan = initVulkan;
        MirageSystem::initOpenGl = initOpenGl;
        
        if (initVulkan)
        {
            if (!glfwVulkanSupported())
            {
                throw gcnew System::Exception("Vulkan not supported");
            }
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            InitVulkan();
            System::Console::WriteLine("Vulkan initialized");
        }
        else if (initOpenGl)
        {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef _DEBUG
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
            
            System::Console::WriteLine("OpenGl initialized");
        }
        else
        {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }
        
        System::Console::WriteLine("GLFW Version: " + gcnew System::String(glfwGetVersionString()));
    
        if (initOpenGl)
        {
            System::Console::WriteLine("Initializing OpenGL context");
        }
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
