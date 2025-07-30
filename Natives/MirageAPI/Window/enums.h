#pragma once

namespace MirageAPI::Window
{
    public enum class WindowType : unsigned long
    {
        Overlapped = WS_OVERLAPPEDWINDOW,
        Fullscreen = WS_OVERLAPPEDWINDOW,
        Popup = WS_POPUPWINDOW,
        Tiled = WS_TILEDWINDOW
    };

}
