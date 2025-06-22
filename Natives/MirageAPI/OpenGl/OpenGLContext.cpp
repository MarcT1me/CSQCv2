#include "pch.h"
#include "OpenGLContext.h"

#include <Gl/glew.h>
#include <GLFW/glfw3.h>

namespace MirageAPI::OpenGL
{
    void OpenGLContext::Initialize(Window::NativeWindow^ win)
    {
        win->MakeCurrent();

        glewExperimental = GL_TRUE;
        GLenum glewInitStatus = glewInit();

        if (glewInitStatus != GLEW_OK)
        {
            const char* error = reinterpret_cast<const char*>(glewGetErrorString(glewInitStatus));
            throw gcnew System::Exception("Ошибка GLEW: " + *error);
        }

        System::Console::WriteLine(
            "GLEW Version: " + *glewGetString(GLEW_VERSION)
        );
        System::Console::WriteLine(
            "OpenGL Version: " + *glGetString(GL_VERSION)
        );
        System::Console::WriteLine(
            "GLSL Version: " + *glGetString(GL_SHADING_LANGUAGE_VERSION)
        );

        if (!GLEW_VERSION_3_3)
        {
            throw gcnew System::Exception("OpenGL 3.3+ is not supported");
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            System::Console::WriteLine("OpenGL error after initialization: " + err);
        }
    }

    void OpenGLContext::ClearBuffers()
    {
        GLFWwindow* current = glfwGetCurrentContext();
        if (!current)
        {
            System::Console::WriteLine("Warning: No OpenGL context current during ClearBuffers!");
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            System::Console::WriteLine("OpenGL error in ClearBuffers: " + err);
        }
    }

    void OpenGLContext::SetViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
}
