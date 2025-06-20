#include "pch.h"
#include "NativeWindow.h"

#include <iostream>
#include <msclr/marshal_cppstd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "NativeEventManager.h"

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
        msclr::interop::marshal_context context;
        std::string nativeTitle = context.marshal_as<std::string>(title);

        glfw_window = glfwCreateWindow(
            width, height, nativeTitle.c_str(),
            nullptr, parent ? parent->glfw_window : nullptr
        );

        if (!glfw_window)
        {
            throw gcnew System::Exception("Не удалось создать окно GLFW");
        }

        gch = System::Runtime::InteropServices::GCHandle::Alloc(this);
        void* native_ptr = System::Runtime::InteropServices::GCHandle::ToIntPtr(gch).ToPointer();

        glfwSetWindowUserPointer(glfw_window, native_ptr);

        Events::NativeEventManager::InitializeCallbacks(glfw_window);
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

    System::IntPtr NativeWindow::Handle::get()
    {
        return System::IntPtr(glfw_window);
    }
}
