#pragma once

namespace MirageAPI
{
    public ref struct DisplayInfo
    {
        IntPtr Handle;
        String^ Name;
        Vector2i Size;
        Vector2i Position;
        bool IsPrimary;
    };
}