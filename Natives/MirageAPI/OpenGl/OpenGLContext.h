#pragma once

#include "../Window/NativeWindow.h"

namespace MirageAPI::OpenGL
{
    public ref class OpenGLContext
    {
    public:
        static void Initialize(Window::NativeWindow^);

        static void ClearBuffers();
        static void SetViewport(int x, int y, int width, int height);
    };
}
