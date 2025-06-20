#pragma once

#include "NativeEvents.h"

struct GLFWwindow;

namespace MirageAPI::Window
{
    public ref class NativeWindow
    {
        GLFWwindow* glfw_window;
        System::Runtime::InteropServices::GCHandle gch;

    public:
        // rise methods (overhead...)
        void RaiseKeyEvent(Events::NativeKeyEvent event);
        void RaiseMouseEvent(Events::NativeMouseEvent event);
        void RaiseWindowEvent(Events::NativeWindowEvent event);
        void RaiseCharEvent(Events::NativeCharEvent event);
        void RaiseDropEvent(Events::NativeDropEvent event);
        
        // Keyboard

        delegate void KeyDelegate(Events::NativeKeyEvent event);
        event KeyDelegate^ OnKey;

        // Mouse

        delegate void MouseEventDelegate(Events::NativeMouseEvent event);
        event MouseEventDelegate^ OnMouse;

        // Window events

        delegate void WindowEventDelegate(Events::NativeWindowEvent event);
        event WindowEventDelegate^ OnWindow;

        // other

        delegate void CharDelegate(Events::NativeCharEvent event);
        // GLFWwindow* window, unsigned int codepoint
        event CharDelegate^ OnChar;

        delegate void DropDelegate(Events::NativeDropEvent event);
        // GLFWwindow* window, int count, const char** paths
        event DropDelegate^ OnDrop;

        // initializations and property

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
    };
}
