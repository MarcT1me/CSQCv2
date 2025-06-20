#include "pch.h"
#include "NativeWindow.h"

#include <iostream>
#include <msclr/marshal_cppstd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace MirageAPI::Window
{
    // event handling

    void NativeWindow::GLFW_KeyCallback(
        GLFWwindow* window,
        const int key, const int scancode,
        const int action, const int mods
    )
    {
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        GlfwEvent e;
        e.Type = GlfwEventType::Key;
        e.Key = key;
        e.Scancode = scancode;
        e.Action = action;
        e.Mods = mods;

        auto handle = System::Runtime::InteropServices::GCHandle::FromIntPtr(System::IntPtr(ptr));
        auto wrapper = safe_cast<NativeWindow^>(handle.Target);

        wrapper->RaiseEvent(e);
    }

    void NativeWindow::GLFW_WindowSizeCallback(
        GLFWwindow* window,
        const int width, const int height
    )
    {
        void* ptr = glfwGetWindowUserPointer(window);
        if (!ptr) return;

        GlfwEvent e;
        e.Type = GlfwEventType::WindowResize;
        e.Width = width;
        e.Height = height;

        auto handle = System::Runtime::InteropServices::GCHandle::FromIntPtr(System::IntPtr(ptr));
        auto wrapper = safe_cast<NativeWindow^>(handle.Target);
        wrapper->RaiseEvent(e);
    }

    // creating and deleting

    NativeWindow::NativeWindow(
        const int width, const int height,
        System::String^ title,
        NativeWindow^ parent
    )
    {
        msclr::interop::marshal_context context;
        std::string nativeTitle = context.marshal_as<std::string>(title);

        glfw_window = glfwCreateWindow(
            width, height, nativeTitle.c_str(),
            nullptr, parent ? parent->glfw_window : nullptr
        );

        if (!glfw_window)
        {
            glfwTerminate();
            throw gcnew System::Exception("Не удалось создать окно GLFW");
        }

        gch = System::Runtime::InteropServices::GCHandle::Alloc(this);

        void* native_ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(gch).ToPointer();
        glfwSetWindowUserPointer(glfw_window, native_ptr);
        glfwSetKeyCallback(
            glfw_window,
            reinterpret_cast<GLFWkeyfun>(GLFW_KeyCallback)
        );
        glfwSetWindowSizeCallback(
            glfw_window,
            reinterpret_cast<GLFWwindowsizefun>(GLFW_WindowSizeCallback)
        );
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

    void NativeWindow::InitGLContext()
    {
        glfwMakeContextCurrent(glfw_window);

        GLenum glewInitStatus = glewInit();

        if (glewInitStatus != GLEW_OK)
        {
            System::String^ error = gcnew System::String(
                reinterpret_cast<const char*>(glewGetErrorString(glewInitStatus))
            );
            throw gcnew System::Exception("Ошибка GLEW: " + error);
        }

        System::String^ version = gcnew System::String(
            reinterpret_cast<const char*>(glewGetString(GLEW_VERSION))
        );
        System::Console::WriteLine("Используется GLEW: " + version);
    }

    // props

    System::IntPtr NativeWindow::Handle::get()
    {
        return System::IntPtr(glfw_window);
    }

    // other methods

    void NativeWindow::RaiseEvent(GlfwEvent e)
    {
        OnGlfwEvent(e);
    }

    void NativeWindow::PollEvents()
    {
        glfwPollEvents();
    }
}
