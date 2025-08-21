#include "pch.h"
#include "DisplayManager.h"

namespace MirageAPI
{
    BOOL CALLBACK MonitorEnumProc(
        HMONITOR hMonitor,
        HDC hdcMonitor,
        LPRECT lprcMonitor,
        LPARAM dwData
    )
    {
        MONITORINFOEX info;
        info.cbSize = sizeof(MONITORINFOEX);
        GetMonitorInfo(hMonitor, &info);

        DisplayInfo^ displayInfo = gcnew DisplayInfo(
            IntPtr(hMonitor),
            gcnew String(info.szDevice),
            gcnew Rect(info.rcMonitor),
            info.dwFlags & MONITORINFOF_PRIMARY
        );

        DisplayManager::DisplayList->Add(displayInfo);
        return TRUE;
    }

    CSList<DisplayInfo^>^ DisplayManager::GetAllDisplays()
    {
        return DisplayList->Count == 0 ? UpdateDisplayList() : DisplayList;
    }

    CSList<DisplayInfo^>^ DisplayManager::UpdateDisplayList()
    {
        DisplayList->Clear();
        EnumDisplayMonitors(
            nullptr, nullptr,
            MonitorEnumProc,
            0
        );
        return DisplayList;
    }

    DisplayInfo^ DisplayManager::GetPrimaryDisplay()
    {
        if (DisplayList->Capacity == 0) UpdateDisplayList();
        for each (auto monitor in DisplayList)
        {
            if (monitor->isPrimary)
                return monitor;
        }
        throw gcnew QuantumFailure("Primary Monitor not found");
    }

    DisplayInfo^ DisplayManager::GetDisplayFromHandle(IntPtr hMonitor)
    {
        if (DisplayList->Capacity == 0) UpdateDisplayList();
        for each (auto monitor in DisplayList)
        {
            if (monitor->handle == hMonitor) return monitor;
        }
        throw gcnew QuantumFailure("Monitor for " + hMonitor + " not found");
    }
}
