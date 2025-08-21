#pragma once

using namespace System;
using namespace OpenTK::Mathematics;

#include "../Window/Window.h"

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
        static void MouseMoveCallback(Window^ window, int xPos, int yPos);
        static void MouseLeaveCallback(Window^ window);
        static void MouseEnterCallback(Window^ window);
        // windowed mouse
        static void MouseButtonCallback(
            Window^ window,
            int button,
            bool pressed,
            int mods
        );
        static void MouseScrollCallback(Window^ window, int delta);

        // window
        static void WindowSizeCallback(Window^ window, int width, int height, int state, bool isFinal);
        static void WindowMoveCallback(Window^ window, int x, int y, bool isFinal);
        // flag only
        static void WindowFocusCallback(Window^ window, bool focused);
        static void WindowVisibilityCallback(Window^ window, bool visible);
        // simple
        static void WindowCreateCallback(Window^ window);
        static void WindowCloseCallback(Window^ window);
        static void WindowDestroyCallback(Window^ window);
        static void WindowDisplayCallback(Window^ window);
        static void WindowDpiCallback(Window^ window, int dpi, Rect^ rect);

        static void PostEvent(
            HWND hwnd, UINT eventType,
            WPARAM wParam, LPARAM lParam
        );
        
        // key handling (maybe without window)
        static void KeyEventHandle(KeyEvent^ event);

    public:
        delegate void KeyDelegate(KeyEvent^ event);
        static event KeyDelegate^ OnKey;
        
        static void ProcessEvents();

        static void PostQuit(int value);

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
