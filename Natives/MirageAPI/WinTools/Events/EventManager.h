#pragma once

using namespace System;
using namespace OpenTK::Mathematics;

#include "../Window.h"

namespace MirageAPI::Events
{
    public ref class EventManager
    {
    internal:
        // key

        static void KeyCallback(
            Window^ window,
            int key,
            bool pressed,
            int scancode,
            int mods
        );

        // mouse

        static void MouseButtonCallback(
            Window^ window,
            int button,
            bool pressed,
            int mods
        );

        static void ScrollCallback(Window^ window, int params);
        static void CursorPositionCallback(Window^ window, int xPos, int yPos);
        static void CursorLeaveCallback(Window^ window);
        static void CursorEnterCallback(Window^ window);

        // window

        static void WindowFocusedCallback(Window^ window, int focused);
        static void WindowMaximizeCallback(Window^ window, int maximize);
        static void WindowIconifyCallback(Window^ window, int iconify);
        static void WindowResizeCallback(Window^ window, int width, int height);
        static void WindowMoveCallback(Window^ window, int x, int y);
        static void WindowRefreshCallback(Window^ window);
        static void WindowCloseCallback(Window^ window);

        static void PostEvent(
            HWND hwnd, UINT eventType,
            WPARAM wParam, LPARAM lParam
        );

    public:
        static void ProcessEvents();

        static void PostEvent(
            UINT eventType,
            WPARAM wParam, LPARAM lParam
        );
        static void PostEvent(
            Window^ window, UINT eventType,
            WPARAM wParam, LPARAM lParam
        );

        static void SendEvent(
            Window^ window, UINT eventType,
            WPARAM wParam, LPARAM lParam
        );
    };
}
