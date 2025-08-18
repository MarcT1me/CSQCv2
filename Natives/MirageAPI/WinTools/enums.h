#pragma once

namespace MirageAPI
{
    public enum class WindowType : unsigned long
    {
        Overlapped = WS_OVERLAPPEDWINDOW | WS_EX_LAYERED,
        Fullscreen = WS_OVERLAPPED,
        Popup = WS_POPUPWINDOW | WS_EX_LAYERED,
        Child = WS_CHILDWINDOW
    };
}
