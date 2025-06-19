#pragma once

struct GLFWwindow;

namespace MirageAPI::Window
{
    public ref class NativeWindow
    {
        GLFWwindow* window_ptr;

    public:
        NativeWindow(
            int width, int height,
            System::String^ title,
            NativeWindow^ parent
        );
        ~NativeWindow();
        !NativeWindow();

        property System::IntPtr Handle
        {
            System::IntPtr get();
        }

        void InitGLContext();
    };
}
