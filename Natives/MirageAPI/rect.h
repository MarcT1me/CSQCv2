#pragma once

namespace MirageAPI
{
    public value struct SimpleRect
    {
        float X, Y, Width, Height;
    };

    public value struct WindowRect
    {
        int Right, Left, Top, Bottom;
        int X, Y, Width, Height;

        static WindowRect^ FromSimple(SimpleRect^ rect)
        {
            auto wRect = gcnew WindowRect();
            wRect->X = wRect->X;
            wRect->Y = wRect->Y;
            wRect->Width = rect->Width;
            wRect->Height = rect->Height;
            return wRect;
        }
    };
}
