#pragma once

#include "enums.h"

namespace MirageAPI
{
    ref class Window;
    ref class CursorInfo;
    ref class IconInfo;
    ref struct DisplayInfo;
}

namespace MirageAPI
{
    public ref class WindowStyleInfo
    {
        // attached window
        Window^ window;
        // style
        bool isDarkMode;
        bool pixelTransparency;
        Byte opacity;
        SystemBackdropType backdropType;
        Color4^ borderColor;
        Color4^ captionColor;
        // info classes
        IconInfo^ icon;
        CursorInfo^ cursor;

    internal:
        WindowStyleInfo^ LinkToWindow(Window^ window);
        void SetStyleForType(WindowType wType);

    public:
        WindowStyleInfo(
            bool isDarkMode,
            bool pixelTransparency,
            Byte opacity,
            SystemBackdropType backdropType,
            Color4^ borderColor,
            Color4^ captionColor,
            IconInfo^ icon,
            CursorInfo^ cursor
        );

        property bool DarkMode
        {
            bool get() { return isDarkMode; }
            void set(bool value);
        }

        property Byte Opacity
        {
            Byte get() { return opacity; }
            void set(Byte value);
        }
        property bool PixelTransparency
        {
            bool get() { return pixelTransparency; }
            void set(bool value);
        }

        property SystemBackdropType BackdropType
        {
            SystemBackdropType get() { return backdropType; }
            void set(SystemBackdropType value);
        }

        property Color4^ BorderColor
        {
            Color4^ get() { return borderColor; }
            void set(Color4^ value);
        }
        property Color4^ CaptionColor
        {
            Color4^ get() { return captionColor; }
            void set(Color4^ value);
        }

        property IconInfo^ Icon
        {
            IconInfo^ get() { return icon; }
            void set(IconInfo^ value);
        }
        property CursorInfo^ Cursor
        {
            CursorInfo^ get() { return cursor; }
            void set(CursorInfo^ value);
        }
    };
}
