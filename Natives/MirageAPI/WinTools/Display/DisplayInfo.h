#pragma once

namespace MirageAPI
{
    public ref struct DisplayInfo
    {
    internal:
        IntPtr handle;
        String^ name;
        Rect^ rect;
        bool isPrimary;

    public:
        DisplayInfo(
            IntPtr handle,
            String^ name,
            Rect^ rect,
            bool isPrimary
        ) : handle(handle),
            name(name),
            rect(rect),
            isPrimary(isPrimary)
        {
        }

        property IntPtr Handle
        {
            IntPtr get() { return handle; }
        }
        property String^ Name
        {
            String^ get() { return name; }
        }
        property Rect^ Rect
        {
            MirageAPI::Rect^ get() { return rect; }
        }
        property bool IsPrimary
        {
            bool get() { return isPrimary; }
        }
    };
}
