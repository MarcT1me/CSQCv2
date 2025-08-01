#pragma once

namespace MirageAPI
{
    public value struct SimpleRect
    {
        float X, Y, Width, Height;
    };

    public value struct DoubleRect
    {
        int Right, Left, Top, Bottom;
        int X, Y, Width, Height;

        static DoubleRect^ FromSimple(SimpleRect^ rect)
        {
            auto wRect = gcnew DoubleRect();
            wRect->X = wRect->X;
            wRect->Y = wRect->Y;
            wRect->Width = static_cast<int>(rect->Width);
            wRect->Height = static_cast<int>(rect->Height);
            return wRect;
        }
    };
}
