#include "pch.h"
#include "EventManager.h"

#define MaybeEmptyWindow(window) (window) ? (window)->Handle : IntPtr();

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
        KeyEvent^ e = gcnew KeyEvent();
        e->windowID = MaybeEmptyWindow(window);

        e->Key = key;
        e->Pressed = pressed;

        e->Scancode = scancode;
        e->Mods = mods;

        // raise event
        KeyEventHandle(e);
    }

    // mouse

    void EventManager::MouseMoveCallback(Window^ window, int xPos, int yPos)
    {
        // format event
        MouseEvent^ e = gcnew MouseEvent();
        e->windowID = MaybeEmptyWindow(window);
        e->Type = EventType::MouseMove;

        e->Pos = Vector2i(xPos, yPos);
        e->Rel = *Mouse::DeltaForDisplay;

        // raise event
        Mouse::MouseEventHandle(window, e);
    }

    void EventManager::MouseLeaveCallback(Window^ window)
    {
        // format event
        MouseEvent^ e = gcnew MouseEvent();
        e->windowID = MaybeEmptyWindow(window);
        e->Type = EventType::MouseLeave;

        // raise event
        Mouse::MouseEventHandle(window, e);
    }

    void EventManager::MouseEnterCallback(Window^ window)
    {
        // format event
        MouseEvent^ e = gcnew MouseEvent();
        e->windowID = MaybeEmptyWindow(window);
        e->Type = EventType::MouseEnter;

        // raise event
        Mouse::MouseEventHandle(window, e);
    }

    // windowed mouse
    
    void EventManager::MouseButtonCallback(
        Window^ window,
        int button,
        bool pressed,
        int mods
    )
    {
        // format event
        MouseEvent^ e = gcnew MouseEvent();
        e->windowID = window->Handle;
        e->Type = EventType::MouseButton;

        e->Button = button;
        e->Pressed = pressed;

        e->Mode = mods;

        // raise event
        Mouse::MouseEventHandle(window, e);
    }

    void EventManager::MouseScrollCallback(Window^ window, int delta)
    {
        // format event
        MouseEvent^ e = gcnew MouseEvent();
        e->windowID = window->Handle;
        e->Type = EventType::MouseScroll;

        e->Button = delta;

        // raise event
        Mouse::MouseEventHandle(window, e);
    }

    // window

    void EventManager::WindowSizeCallback(Window^ window, int width, int height, int state, bool isFinal)
    {
        // format event
        WindowEvent^ e = gcnew WindowEvent();
        e->windowID = window->Handle;
        e->Type = EventType::WindowResize;

        e->Flag = isFinal;
        e->State = state;
        e->X = width;
        e->Y = height;

        // raise event
        window->WindowEventHandle(e);
    }

    void EventManager::WindowMoveCallback(Window^ window, int x, int y, bool isFinal)
    {
        // format event
        WindowEvent^ e = gcnew WindowEvent();
        e->windowID = window->Handle;
        e->Type = EventType::WindowMove;

        e->Flag = isFinal;
        e->X = x;
        e->Y = y;

        // raise event
        window->WindowEventHandle(e);
    }
    
    // flag only

    void EventManager::WindowFocusCallback(Window^ window, bool focused)
    {
        // format event
        WindowEvent^ e = gcnew WindowEvent();
        e->windowID = window->Handle;
        e->Type = EventType::WindowFocus;

        e->Flag = focused;

        // raise event
        window->WindowEventHandle(e);
    }

    void EventManager::WindowVisibilityCallback(Window^ window, bool visible)
    {
        // format event
        WindowEvent^ e = gcnew WindowEvent();
        e->windowID = window->Handle;
        e->Type = EventType::WindowVisibility;

        e->Flag = visible;

        // raise event
        window->WindowEventHandle(e);
    }

    // simple
    
    void EventManager::WindowCreateCallback(Window^ window)
    {
        // format event
        WindowEvent^ e = gcnew WindowEvent();
        e->windowID = window->Handle;
        e->Type = EventType::WindowCreate;

        // raise event
        window->WindowEventHandle(e);
    }

    void EventManager::WindowCloseCallback(Window^ window)
    {
        // format event
        WindowEvent^ e = gcnew WindowEvent();
        e->windowID = window->Handle;
        e->Type = EventType::WindowClose;

        // raise event
        window->WindowEventHandle(e);
    }

    void EventManager::WindowDestroyCallback(Window^ window)
    {
        // format event
        WindowEvent^ e = gcnew WindowEvent();
        e->windowID = window->Handle;
        e->Type = EventType::WindowDestroy;

        // raise event
        window->WindowEventHandle(e);
    }

    void EventManager::WindowDisplayCallback(Window^ window)
    {
        WindowEvent^ e = gcnew WindowEvent();
        e->windowID = window->Handle;
        e->Type = EventType::WindowDisplayChange;

        window->WindowEventHandle(e);
    }

    void EventManager::WindowDpiCallback(Window^ window, int dpi, Rect^ rect)
    {
    }

    // key handling (maybe without window)
    void EventManager::KeyEventHandle(KeyEvent^ event)
    {
        OnKey(event);
    }

    // event queue methods
    
    void EventManager::ProcessEvents()
    {
        MSG msg = {nullptr};
        Mouse::UpdateDisplayPositions();
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    void EventManager::PostQuit(int value)
    {
        PostQuitMessage(value);
    }

    void EventManager::PostEvent(
        UINT eventType,
        WPARAM wParam, LPARAM lParam
    )
    {
        for (int i = 0; i < Window::winList->Count; i++)
        {
            PostEvent(
                Window::winList[i]->NativeWindow,
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
            window->NativeWindow,
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
            window->NativeWindow,
            eventType, wParam, lParam | HTCLIENT
        );
    }
}
