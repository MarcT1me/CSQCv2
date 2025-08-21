#pragma once

#include "enums.h"
#include "../../NativeInstance.h"
#include "../Cursor/CursorInfo.h"
#include "../Icon/IconInfo.h"

namespace MirageAPI
{
    ref class Window;
    ref class NativeInstance;
}

namespace MirageAPI::Events
{
    ref class EventProc;
}

namespace MirageAPI
{
    public ref struct WindowClass
    {
    private:
        int name;
        NativeInstance^ instance;
        Events::EventProc^ eventProc;

        CursorInfo^ cursor;
        IconInfo^ icon;

    public:
        static WindowClass^ Default = gcnew WindowClass(
            1, NativeInstance::MirageAPI, nullptr,
            CursorInfo::QDefault, IconInfo::QDefault
        ); // default QuantumWindow class

        static WindowClass()
        {
            SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        }

        static int DefaultExStyle = CS_HREDRAW | CS_VREDRAW;

        WindowClass(
            int name,
            NativeInstance^ instance,
            Events::EventProc^ proc,
            CursorInfo^ cursor,
            IconInfo^ icon
        );

        ~WindowClass() { this->!WindowClass(); }
        !WindowClass();

        property int Name
        {
            int get() { return name; }
        }
        property NativeInstance^ Instance
        {
            NativeInstance^ get() { return instance; }
        }
        property Events::EventProc^ EventProc
        {
            Events::EventProc^ get() { return eventProc; }
        }
        property CursorInfo^ Cursor
        {
            CursorInfo^ get() { return cursor; }
        }
        property IconInfo^ Icon
        {
            IconInfo^ get() { return icon; }
        }

        void SetCursor(Window^ window, CursorInfo^ cursor);
        void SetIcon(Window^ window, IconInfo^ icon);
    };
}
