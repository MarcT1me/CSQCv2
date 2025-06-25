#pragma once

#include "../Window/NativeWindow.h"

namespace MirageAPI::Events
{
    public ref class NativeEventManager
    {
    internal:
        // key

        static void KeyCallback(
            Window::NativeWindow^ window,
            int key, int scancode,
            int action, int mods
        );

        // mouse

        static void MouseButtonCallback(
            Window::NativeWindow^ window,
            int button,
            int action, int mods
        );

        static void ScrollCallback(Window::NativeWindow^ window, double xOffset, double yOffset);
        static void CursorPositionCallback(Window::NativeWindow^ window, double xPos, double yPos);

        // window

        static void WindowFocusedCallback(Window::NativeWindow^ window, int focused);
        static void WindowMaximizeCallback(Window::NativeWindow^ window, int maximize);
        static void WindowIconifyCallback(Window::NativeWindow^ window, int iconify);
        static void WindowResizeCallback(Window::NativeWindow^ window, int width, int height);
        static void WindowMoveCallback(Window::NativeWindow^ window, int x, int y);
        static void WindowRefreshCallback(Window::NativeWindow^ window);
        static void WindowCloseCallback(Window::NativeWindow^ window);

    public:
        static void ProcessEvents();
    };
}
