#include "pch.h"
#include "WindowClass.h"

#include "../Events/EventProc.h"
#include "../../NativeInstance.h"

namespace MirageAPI
{
    WindowClass::WindowClass(
        int name,
        NativeInstance^ instance,
        Events::EventProc^ proc,
        CursorInfo^ cursor,
        IconInfo^ icon
    ) : name(name),
        instance(instance),
        eventProc(proc ? proc : Events::EventProc::Default),
        cursor(cursor),
        icon(icon)
    {
        WNDCLASSEX QWindow = {};
        // constant data
        QWindow.cbSize = sizeof(WNDCLASSEX);
        QWindow.style = DefaultExStyle;
        QWindow.hInstance = instance->hInstance;
        QWindow.lpfnWndProc = reinterpret_cast<WNDPROC>(Events::EventProc::QuantumProc);
        QWindow.lpszClassName = MAKEINTRESOURCEW(name);

        if (cursor)
        {
            QWindow.hCursor = cursor->NativeCursor;
        }
        if (icon)
        {
            QWindow.hIcon = icon->NativeIcon;
            QWindow.hIconSm = icon->NativeIcon;
        }

        // register class
        if (!RegisterClassEx(&QWindow))
        {
            throw gcnew MirageAPIException("Window class registration error: " + GetLastError());
        }
    }

    void WindowClass::!WindowClass()
    {
        // unregister class
        if (!UnregisterClassW(MAKEINTRESOURCEW(name), instance->hInstance))
        {
            DWORD error = GetLastError();
            throw gcnew MirageAPIException("Window class deletion error: " + error);
        }
    }

    void WindowClass::SetCursor(Window^ window, CursorInfo^ value)
    {
        cursor = value;

        CheckNull(value) return SetCursor(window, CursorInfo::Default);
        
        SetClassLongPtrW(
            window->NativeWindow, GCLP_HCURSOR,
            reinterpret_cast<LONG_PTR>(value->NativeCursor)
        );
    }

    void WindowClass::SetIcon(Window^ window, IconInfo^ value)
    {
        icon = value;
        
        CheckNull(value) return SetIcon(window, IconInfo::Default);
        
        SetClassLongPtrW(
            window->NativeWindow, GCLP_HICON,
            reinterpret_cast<LONG_PTR>(value->NativeIcon)
        );
        SetClassLongPtrW(
            window->NativeWindow, GCLP_HICONSM,
            reinterpret_cast<LONG_PTR>(value->NativeIcon)
        );
    }
}
