#include "pch.h"
#include "NativeWindow.h"

#include <msclr/marshal_cppstd.h>

#include <GLFW/glfw3.h>

#include "../MirageAPI.h"
#include "../Events/NativeEventManager.h"
#include "../OpenGl/OpenGLContext.h"

namespace MirageAPI::Window
{
    // event rises
    void NativeWindow::RaiseKeyEvent(Events::NativeKeyEvent event)
    {
        OnKey(event);
    }

    void NativeWindow::RaiseMouseEvent(Events::NativeMouseEvent event)
    {
        OnMouse(event);
    }

    void NativeWindow::RaiseWindowEvent(Events::NativeWindowEvent event)
    {
        OnWindow(event);
    }

    void NativeWindow::RaiseCharEvent(Events::NativeCharEvent event)
    {
        OnChar(event);
    }

    void NativeWindow::RaiseDropEvent(Events::NativeDropEvent event)
    {
        OnDrop(event);
    }

    // initializations and property

    NativeWindow::NativeWindow(
        const int width, const int height,
        System::String^ title,
        NativeWindow^ parent
    )
    {
        if (width <= 0 || height <= 0)
        {
            throw gcnew System::ArgumentException("Invalid window size");
        }
        
        msclr::interop::marshal_context context;
        std::string nativeTitle = context.marshal_as<std::string>(title);

        glfw_window = glfwCreateWindow(
            width, height, nativeTitle.c_str(),
            nullptr, parent ? parent->glfw_window : nullptr
        );

        if (!glfw_window)
        {
            const char* message;
            glfwGetError(&message);
            throw gcnew System::Exception("Не удалось создать окно GLFW" + *message);
        }

        gch = System::Runtime::InteropServices::GCHandle::Alloc(this);
        void* native_ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(gch).ToPointer();

        glfwSetWindowUserPointer(glfw_window, native_ptr);

        Events::NativeEventManager::InitializeCallbacks(glfw_window);

        if (MirageSystem::initOpenGl)
            OpenGL::OpenGLContext::Initialize(this);
    }

    NativeWindow::~NativeWindow()
    {
        if (glfw_window)
        {
            if (gch.IsAllocated)
            {
                gch.Free();
            }
            glfwDestroyWindow(glfw_window);
            glfw_window = nullptr;
        }
    }

    NativeWindow::!NativeWindow()
    {
    }

    System::IntPtr NativeWindow::Handle::get()
    {
        return System::IntPtr(glfw_window);
    }

    void NativeWindow::CreateVulkanSurface(VkInstance instance)
    {
        VkSurfaceKHR surf;
        if (glfwCreateWindowSurface(instance, glfw_window, nullptr, &surf) != VK_SUCCESS)
        {
            throw gcnew System::Exception("Failed to create window surface!");
        }
        surface = surf;
    }

    void NativeWindow::CleanupVulkanSurface(VkInstance instance)
    {
        if (surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(instance, surface, nullptr);
            surface = VK_NULL_HANDLE;
        }
    }

    void NativeWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
    }

    void NativeWindow::SetViewport(int x, int y, int width, int height)
    {
        OpenGL::OpenGLContext::SetViewport(x, y, width, height);
    }

    void NativeWindow::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        OpenGL::OpenGLContext::ClearBuffers();
    }
    
    void NativeWindow::MakeCurrent()
    {
        glfwMakeContextCurrent(glfw_window);
    }

    void NativeWindow::SwapBuffers()
    {
        MakeCurrent();
        glfwSwapBuffers(glfw_window);
    }

    bool NativeWindow::ShouldClose()
    {
        return glfwWindowShouldClose(glfw_window) == GLFW_TRUE;
    }

    void NativeWindow::SetSize(int width, int height)
    {
        glfwSetWindowSize(glfw_window, width, height);
    }

    void NativeWindow::SetSizeLimit(int minWidth, int minHeight, int maxWidth, int maxHeight)
    {
        glfwSetWindowSizeLimits(glfw_window, minWidth, minHeight, maxWidth, maxHeight);
    }

    void NativeWindow::SetPos(int xPos, int yPos)
    {
        glfwSetWindowPos(glfw_window, xPos, yPos);
    }

    void NativeWindow::SetOpacity(float opacity)
    {
        glfwSetWindowOpacity(glfw_window, opacity);
    }

    void NativeWindow::Focus()
    {
        glfwFocusWindow(glfw_window);
    }

    void NativeWindow::Show()
    {
        glfwShowWindow(glfw_window);
    }

    void NativeWindow::Hide()
    {
        glfwHideWindow(glfw_window);
    }

    void NativeWindow::Maximize()
    {
        glfwMaximizeWindow(glfw_window);
    }

    void NativeWindow::Restore()
    {
        glfwRestoreWindow(glfw_window);
    }
}
