#pragma once

#include "../Window/Window.h"

namespace MirageAPI::Events
{
    public ref class EventProc
    {
        static Window^ GetNativeWindow(HWND hwnd);
        static long long DefaultProc(Window^ window, UINT eventType, EventParams^ params);

    internal:
        static LRESULT QuantumProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    public:
        static EventProc^ Default = gcnew EventProc();
        
        virtual long long Proc(Window^ window, UINT eventType, EventParams^ params);
    };
}
