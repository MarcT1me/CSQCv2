#pragma once

namespace MirageAPI
{
    public ref struct SimpleRect
    {
        float X, Y, Width, Height;
    };

    public ref struct DoubleRect
    {
        int Right, Left, Top, Bottom;
        int X, Y, Width, Height;

        static DoubleRect^ FromSimple(SimpleRect^ rect)
        {
            auto wRect = gcnew DoubleRect();
            wRect->X = static_cast<int>(rect->X);
            wRect->Y = static_cast<int>(rect->Y);
            wRect->Width = static_cast<int>(rect->Width);
            wRect->Height = static_cast<int>(rect->Height);
            return wRect;
        }
    };
}
