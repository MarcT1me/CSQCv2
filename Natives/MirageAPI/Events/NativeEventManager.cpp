#include "pch.h"
#include "NativeEventManager.h"

namespace MirageAPI::Events
{
    // key

    void NativeEventManager::KeyCallback(
        Window::NativeWindow^ window,
        int key,
        bool pressed,
        int scancode,
        int mods
    )
    {
        // format event
        NativeKeyEvent e;
        e.windowID = window->Handle;

        e.Key = key;
        e.Pressed = pressed;

        e.Scancode = scancode;
        e.Mods = mods;

        // raise event
        window->RaiseKeyEvent(e);
    }

    // mouse

    void NativeEventManager::MouseButtonCallback(
        Window::NativeWindow^ window,
        int button,
        bool pressed,
        int mods
    )
    {
        // format event
        NativeMouseEvent e;
        e.windowID = window->Handle;
        e.Type = NativeMouseEventType::Button;

        e.Button = button;
        e.Pressed = pressed;

        e.Mode = mods;

        // raise event
        window->RaiseMouseEvent(e);
    }

    void NativeEventManager::ScrollCallback(Window::NativeWindow^ window, float xOffset, float yOffset)
    {
        // format event
        NativeMouseEvent e;
        e.windowID = window->Handle;
        e.Type = NativeMouseEventType::Scroll;

        e.X = xOffset;
        e.Y = yOffset;

        // raise event
        window->RaiseMouseEvent(e);
    }

    void NativeEventManager::CursorPositionCallback(Window::NativeWindow^ window, float xPos, float yPos)
    {
        // format event
        NativeMouseEvent e;
        e.windowID = window->Handle;
        e.Type = NativeMouseEventType::Move;

        e.X = xPos;
        e.Y = yPos;

        // raise event
        window->RaiseMouseEvent(e);
    }

    // window

    void NativeEventManager::WindowFocusedCallback(Window::NativeWindow^ window, int focused)
    {
        // format event
        NativeWindowEvent e;
        e.windowID = window->Handle;
        e.Type = NativeWindowEventType::Focus;

        e.X = focused;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void NativeEventManager::WindowMaximizeCallback(Window::NativeWindow^ window, int maximize)
    {
        // format event
        NativeWindowEvent e;
        e.windowID = window->Handle;
        e.Type = NativeWindowEventType::Maximize;

        e.X = maximize;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void NativeEventManager::WindowIconifyCallback(Window::NativeWindow^ window, int iconify)
    {
        // format event
        NativeWindowEvent e;
        e.windowID = window->Handle;
        e.Type = NativeWindowEventType::Iconify;

        e.X = iconify;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void NativeEventManager::WindowResizeCallback(Window::NativeWindow^ window, const int width, const int height)
    {
        // format event
        NativeWindowEvent e;
        e.windowID = window->Handle;
        e.Type = NativeWindowEventType::Resize;

        e.X = width;
        e.Y = height;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void NativeEventManager::WindowMoveCallback(Window::NativeWindow^ window, int x, int y)
    {
        // format event
        NativeWindowEvent e;
        e.windowID = window->Handle;
        e.Type = NativeWindowEventType::Move;

        e.X = x;
        e.Y = y;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void NativeEventManager::WindowRefreshCallback(Window::NativeWindow^ window)
    {
        // format event
        NativeWindowEvent e;
        e.windowID = window->Handle;
        e.Type = NativeWindowEventType::Refresh;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void NativeEventManager::WindowCloseCallback(Window::NativeWindow^ window)
    {
        // format event
        NativeWindowEvent e;
        e.windowID = window->Handle;
        e.Type = NativeWindowEventType::Close;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void NativeEventManager::ProcessEvents()
    {
        MSG msg = {nullptr};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}
