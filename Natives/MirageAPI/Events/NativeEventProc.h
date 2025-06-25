#pragma once

#include <Windows.h>

#include "../Window/NativeWindow.h"

namespace MirageAPI::Events
{
    public ref class NativeEventProc
    {
        static Window::NativeWindow^ GetNativeWindow(HWND hWnd);

    internal:
        static LRESULT CALLBACK QuantumWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}
