#pragma once

#include "../Window.h"

namespace MirageAPI::Events
{
    public ref class EventProc
    {
        static Window^ GetNativeWindow(HWND hWnd);

    internal:
        static LRESULT QuantumWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}
