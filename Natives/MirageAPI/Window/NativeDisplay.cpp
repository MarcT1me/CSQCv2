#include "pch.h"
#include "NativeDisplay.h"

namespace MirageAPI::Window
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

        NativeMonitorInfo^ mi = gcnew NativeMonitorInfo();
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

        NativeDisplay::Monitors->Add(mi);
        return TRUE;
    }

    CSList<NativeMonitorInfo^>^ NativeDisplay::GetAllMonitors()
    {
        return Monitors->Count == 0 ? UpdateMonitors() : Monitors;
    }

    CSList<NativeMonitorInfo^>^ NativeDisplay::UpdateMonitors()
    {
        Monitors->Clear();
        EnumDisplayMonitors(
            nullptr, nullptr,
            MonitorEnumProc,
            0
        );
        return Monitors;
    }

    NativeMonitorInfo^ NativeDisplay::GetPrimaryMonitor()
    {
        if (Monitors->Capacity == 0) UpdateMonitors();
        for each (auto monitor in Monitors)
        {
            if (monitor->IsPrimary)
                return monitor;
        }
        return Monitors[0];
    }

    NativeMonitorInfo^ NativeDisplay::GetMonitorFromHandle(IntPtr hMonitor)
    {
        if (Monitors->Capacity == 0) UpdateMonitors();
        for each (auto monitor in Monitors)
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
