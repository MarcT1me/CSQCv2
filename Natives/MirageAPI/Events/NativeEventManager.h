#pragma once

using namespace System;
using namespace OpenTK::Mathematics;

#include "../Window/NativeWindow.h"

namespace MirageAPI::Events
{
    public ref class NativeEventManager
    {
    internal:
        // key

        static void KeyCallback(
            Window::NativeWindow^ window,
            int key,
            bool pressed,
            int scancode,
            int mods
        );

        // mouse

        static void MouseButtonCallback(
            Window::NativeWindow^ window,
            int button,
            bool pressed,
            int mods
        );

        static void ScrollCallback(Window::NativeWindow^ window, int params);
        static void CursorPositionCallback(Window::NativeWindow^ window, int xPos, int yPos);
        static void CursorLeaveCallback(Window::NativeWindow^ window);
        static void CursorEnterCallback(Window::NativeWindow^ window);

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
