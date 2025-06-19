#include "pch.h"
#include "NativeWindow.h"

#include <iostream>
#include <msclr/marshal_cppstd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace MirageAPI::Window
{
    NativeWindow::NativeWindow(
        const int width, const int height,
        System::String^ title,
        NativeWindow^ parent
    )
    {
        msclr::interop::marshal_context context;
        std::string nativeTitle = context.marshal_as<std::string>(title);

        window_ptr = glfwCreateWindow(
            width, height, nativeTitle.c_str(),
            nullptr, parent ? parent->window_ptr : nullptr
        );

        if (!window_ptr)
        {
            glfwTerminate();
            throw gcnew System::Exception("Не удалось создать окно GLFW");
        }
    }

    NativeWindow::~NativeWindow()
    {
        if (window_ptr)
        {
            glfwDestroyWindow(window_ptr);
            window_ptr = nullptr;
        }
    }

    NativeWindow::!NativeWindow()
    {
    }

    System::IntPtr NativeWindow::Handle::get()
    {
        return System::IntPtr(window_ptr);
    }

    void NativeWindow::InitGLContext()
    {
        glfwMakeContextCurrent(window_ptr);
        
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
}
