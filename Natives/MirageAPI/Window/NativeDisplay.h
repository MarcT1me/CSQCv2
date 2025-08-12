#pragma once

#include "NativeDisplayInfo.h"

namespace MirageAPI::Window
{
    public ref class NativeDisplay abstract
    {
    public:
        static CSList<NativeMonitorInfo^>^ Monitors = gcnew CSList<NativeMonitorInfo^>();

        static CSList<NativeMonitorInfo^>^ GetAllMonitors();

        static CSList<NativeMonitorInfo^>^ UpdateMonitors();
        static NativeMonitorInfo^ GetPrimaryMonitor();
        static NativeMonitorInfo^ GetMonitorFromHandle(IntPtr hMonitor);
    };
}
