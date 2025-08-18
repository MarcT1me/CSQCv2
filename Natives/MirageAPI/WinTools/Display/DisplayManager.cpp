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

        DisplayInfo^ mi = gcnew DisplayInfo();
        mi->Handle = IntPtr(hMonitor);
        mi->Position = Vector2i(
            info.rcMonitor.left,
            info.rcMonitor.top
        );
        mi->Size = Vector2i(
            info.rcMonitor.right - info.rcMonitor.left,
            info.rcMonitor.bottom - info.rcMonitor.top
        );
        mi->IsPrimary = (info.dwFlags & MONITORINFOF_PRIMARY) != 0;
        mi->Name = gcnew String(info.szDevice);

        DisplayManager::DisplayList->Add(mi);
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
            if (monitor->IsPrimary)
                return monitor;
        }
        return DisplayList[0];
    }

    DisplayInfo^ DisplayManager::GetDisplayFromHandle(IntPtr hMonitor)
    {
        if (DisplayList->Capacity == 0) UpdateDisplayList();
        for each (auto monitor in DisplayList)
        {
            if (monitor->Handle == hMonitor) return monitor;
        }
        throw gcnew QuantumFailure(
            CSFormat(
                "Monitor for {0} not found",
                hMonitor
            )
        );
    }
}
