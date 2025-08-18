#include "pch.h"
#include "EventManager.h"

namespace MirageAPI::Events
{
    // key

    void EventManager::KeyCallback(
        Window^ window,
        int key,
        bool pressed,
        int scancode,
        int mods
    )
    {
        // format event
        NativeKeyEvent^ e = gcnew NativeKeyEvent();
        e->windowID = window->Handle;

        e->Key = key;
        e->Pressed = pressed;

        e->Scancode = scancode;
        e->Mods = mods;

        // raise event
        window->RaiseKeyEvent(e);
    }

    // mouse

    void EventManager::MouseButtonCallback(
        Window^ window,
        int button,
        bool pressed,
        int mods
    )
    {
        // format event
        NativeMouseEvent^ e = gcnew NativeMouseEvent();
        e->windowID = window->Handle;
        e->Type = NativeMouseEventType::Button;

        e->Button = button;
        e->Pressed = pressed;

        e->Mode = mods;

        // raise event
        window->RaiseMouseEvent(e);
    }

    void EventManager::ScrollCallback(Window^ window, int params)
    {
        // format event
        NativeMouseEvent^ e = gcnew NativeMouseEvent();
        e->windowID = window->Handle;
        e->Type = NativeMouseEventType::Scroll;

        e->Button = params;

        // raise event
        window->RaiseMouseEvent(e);
    }

    void EventManager::CursorPositionCallback(Window^ window, int xPos, int yPos)
    {
        // format event
        NativeMouseEvent^ e = gcnew NativeMouseEvent();
        e->windowID = window->Handle;
        e->Type = NativeMouseEventType::Move;

        e->Pos = Vector2i(xPos, yPos);
        e->Rel = *window->MouseDelta;

        // raise event
        window->RaiseMouseEvent(e);
    }

    void EventManager::CursorLeaveCallback(Window^ window)
    {
        // format event
        NativeMouseEvent^ e = gcnew NativeMouseEvent();
        e->windowID = window->Handle;
        e->Type = NativeMouseEventType::Leave;

        // raise event
        window->RaiseMouseEvent(e);
    }

    void EventManager::CursorEnterCallback(Window^ window)
    {
        // format event
        NativeMouseEvent^ e = gcnew NativeMouseEvent();
        e->windowID = window->Handle;
        e->Type = NativeMouseEventType::Enter;

        // raise event
        window->RaiseMouseEvent(e);
    }

    // window

    void EventManager::WindowFocusedCallback(Window^ window, int focused)
    {
        // format event
        NativeWindowEvent^ e = gcnew NativeWindowEvent();
        e->windowID = window->Handle;
        e->Type = NativeWindowEventType::Focus;

        e->X = focused;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void EventManager::WindowMaximizeCallback(Window^ window, int maximize)
    {
        // format event
        NativeWindowEvent^ e = gcnew NativeWindowEvent();
        e->windowID = window->Handle;
        e->Type = NativeWindowEventType::Maximize;

        e->X = maximize;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void EventManager::WindowIconifyCallback(Window^ window, int iconify)
    {
        // format event
        NativeWindowEvent^ e = gcnew NativeWindowEvent();
        e->windowID = window->Handle;
        e->Type = NativeWindowEventType::Iconify;

        e->X = iconify;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void EventManager::WindowResizeCallback(Window^ window, const int width, const int height)
    {
        // format event
        NativeWindowEvent^ e = gcnew NativeWindowEvent();
        e->windowID = window->Handle;
        e->Type = NativeWindowEventType::Resize;

        e->X = width;
        e->Y = height;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void EventManager::WindowMoveCallback(Window^ window, int x, int y)
    {
        // format event
        NativeWindowEvent^ e = gcnew NativeWindowEvent();
        e->windowID = window->Handle;
        e->Type = NativeWindowEventType::Move;

        e->X = x;
        e->Y = y;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void EventManager::WindowRefreshCallback(Window^ window)
    {
        // format event
        NativeWindowEvent^ e = gcnew NativeWindowEvent();
        e->windowID = window->Handle;
        e->Type = NativeWindowEventType::Refresh;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void EventManager::WindowCloseCallback(Window^ window)
    {
        // format event
        NativeWindowEvent^ e = gcnew NativeWindowEvent();
        e->windowID = window->Handle;
        e->Type = NativeWindowEventType::Close;

        // raise event
        window->RaiseWindowEvent(e);
    }

    void EventManager::ProcessEvents()
    {
        MSG msg = {nullptr};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void EventManager::PostEvent(
        UINT eventType,
        WPARAM wParam, LPARAM lParam
    )
    {
        for (int i = 0; i < Window::winList->Count; i++)
        {
            PostEvent(
                Window::winList[i]->hwnd,
                eventType, wParam, lParam
            );
        }
    }

    void EventManager::PostEvent(
        Window^ window, UINT eventType,
        WPARAM wParam, LPARAM lParam
    )
    {
        PostEvent(
            window->hwnd,
            eventType, wParam, lParam
        );
    }

    void EventManager::PostEvent(
        HWND hwnd, UINT eventType,
        WPARAM wParam, LPARAM lParam
    )
    {
        if (IsWindowVisible(hwnd))
        {
            PostMessage(
                hwnd,
                eventType, wParam, lParam | HTCLIENT
            );
        }
    }

    void EventManager::SendEvent(
        Window^ window, UINT eventType,
        WPARAM wParam, LPARAM lParam
    )
    {
        SendMessage(
            window->hwnd,
            eventType, wParam, lParam | HTCLIENT
        );
    }
}
