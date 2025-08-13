#pragma once

#include "NativeDisplayInfo.h"

namespace MirageAPI::Window
{
    public ref class NativeDisplay abstract
    {
    public:
        static CSList<NativeDisplayInfo^>^ Monitors = gcnew CSList<NativeDisplayInfo^>();

        static CSList<NativeDisplayInfo^>^ GetAllMonitors();

        static CSList<NativeDisplayInfo^>^ UpdateMonitors();
        static NativeDisplayInfo^ GetPrimaryMonitor();
        static NativeDisplayInfo^ GetMonitorFromHandle(IntPtr hMonitor);
    };
}
