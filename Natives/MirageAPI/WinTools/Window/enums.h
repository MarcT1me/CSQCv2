#pragma once

namespace MirageAPI
{
    [System::Flags]
    public enum class WindowType : unsigned long long
    {
        None = 0,
        Overlapped = WS_OVERLAPPEDWINDOW | WS_EX_LAYERED,
        Fullscreen = WS_OVERLAPPED,
        Popup = WS_POPUPWINDOW | WS_EX_LAYERED,
        Child = WS_CHILDWINDOW
    };
}
