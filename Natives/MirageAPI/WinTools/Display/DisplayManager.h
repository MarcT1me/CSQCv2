#pragma once

#include "DisplayInfo.h"

namespace MirageAPI
{
    public ref class DisplayManager abstract
    {
    public:
        static CSList<DisplayInfo^>^ DisplayList = gcnew CSList<DisplayInfo^>();

        static CSList<DisplayInfo^>^ GetAllDisplays();

        static CSList<DisplayInfo^>^ UpdateDisplayList();
        static DisplayInfo^ GetPrimaryDisplay();
        static DisplayInfo^ GetDisplayFromHandle(IntPtr hMonitor);
    };
}
