#pragma once

namespace MirageAPI::Window
{
    public ref struct NativeMonitorInfo
    {
        IntPtr Handle;
        String^ Name;
        Vector2i Size;
        Vector2i Position;
        bool IsPrimary;
    };
}