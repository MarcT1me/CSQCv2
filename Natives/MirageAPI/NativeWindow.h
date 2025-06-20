#pragma once

struct GLFWwindow;

namespace MirageAPI::Window
{
    public enum class GlfwEventType
    {
        Key, Char, MouseButton, Scroll, CursorPos,
        WindowClose, WindowFocus, WindowResize
    };

    public value struct GlfwEvent
    {
        GlfwEventType Type;
        int Key;
        int Scancode;
        int Action;
        int Mods;
        double X;
        double Y;
        int Width;
        int Height;
        int Focus;
    };

    public ref class NativeWindow
    {
        GLFWwindow* glfw_window;
        System::Runtime::InteropServices::GCHandle gch;

        static void GLFW_KeyCallback(
            GLFWwindow* window,
            int key, int scancode,
            int action, int mods
        );

        static void GLFW_WindowSizeCallback(
            GLFWwindow* window,
            int width, int height
        );

    public:
        delegate void GlfwEventDelegate(GlfwEvent e);
        event GlfwEventDelegate^ OnGlfwEvent;

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

        // other methods
        // void SwapBuffers();
        // void MakeCurrent();

        static void PollEvents();

    internal:
        void RaiseEvent(GlfwEvent e);
    };
}
