#pragma once

using namespace System;
using namespace OpenTK::Mathematics;

namespace MirageAPI::Events
{
    public ref struct NativeKeyEvent
    {
        IntPtr windowID;

        int Key;
        bool Pressed;
        
        int Scancode;
        int Mods;
    };

    public enum class NativeMouseEventType
    {
        Button, Move, Scroll,
    };

    public ref struct NativeMouseEvent
    {
        NativeMouseEventType Type;
        IntPtr windowID;

        int Button;
        bool Pressed;

        Vector2i Pos;
        Vector2i Rel;
        
        int Mode;
    };

    public enum class NativeWindowEventType
    {
        Focus, Maximize, Iconify, Resize, Move, Refresh, Close,
    };

    public ref struct NativeWindowEvent
    {
        NativeWindowEventType Type;
        IntPtr windowID;

        int X;
        int Y;
    };

    public ref struct NativeCharEvent
    {
        IntPtr windowID;

        unsigned int codepoint;
    };

    public ref struct NativeDropEvent
    {
        IntPtr windowID;

        int count;
        const char** paths;
    };

    public enum class NativeEventType
    {
        Joystick, Monitor
    };

    public ref struct NativeJoystickEvent
    {
        int JoystickID;
        
        bool Connected;
    };

    public ref struct NativeJoystickState
    {
        int JoystickID;
        
        int AxesCount;
        const float* Axes;
        
        int ButtonCount;
        const unsigned char* Buttons;
        
        int HatCount;
        const unsigned char* Hats;
    };

    public ref struct NativeEvent
    {
        NativeEventType Type;
        int event;
        IntPtr data;
    };
}
