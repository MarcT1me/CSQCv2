#pragma once

#include <Windows.h>

namespace MirageAPI::Window
{
    public enum class WindowType : unsigned long
    {
        Overlapped = WS_OVERLAPPEDWINDOW,
        Fullscreen = WS_OVERLAPPEDWINDOW,
        Popup = WS_POPUPWINDOW,
        Tiled = WS_TILEDWINDOW
    };

    public value struct WindowRect
    {
        int right, left, top, bottom;
        int x, y, width, height;
    };
}
