#pragma once

#include "../Window/NativeWindow.h"

namespace MirageAPI::Events
{
    public ref class NativeEventProc
    {
        static Window::NativeWindow^ GetNativeWindow(HWND hWnd);

    internal:
        static LRESULT QuantumWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}
