#pragma once

namespace MirageAPI::Events
{
    public value struct NativeKeyEvent
    {
        System::IntPtr windowID;

        int Key;
        int Scancode;

        int Action;
        int Mods;
    };

    public enum class NativeMouseEventType
    {
        Button, Move, Scroll,
    };

    public value struct NativeMouseEvent
    {
        NativeMouseEventType Type;
        System::IntPtr windowID;

        int Button;

        double X;
        double Y;

        int Action;
        int Mode;
    };

    public enum class NativeWindowEventType
    {
        Focus, Maximize, Iconify, Resize, Move, Refresh, Close,
    };

    public value struct NativeWindowEvent
    {
        NativeWindowEventType Type;
        System::IntPtr windowID;

        int X;
        int Y;
    };

    public value struct NativeCharEvent
    {
        System::IntPtr windowID;

        unsigned int codepoint;
    };

    public value struct NativeDropEvent
    {
        System::IntPtr windowID;

        int count;
        const char** paths;
    };

    public enum class NativeEventType
    {
        Joystick, Monitor
    };

    public value struct NativeJoystickEvent
    {
        int JoystickID;
        
        bool Connected; // GLFW_CONNECTED или GLFW_DISCONNECTED
    };

    public value struct NativeJoystickState
    {
        int JoystickID;
        
        int AxesCount;
        const float* Axes;
        
        int ButtonCount;
        const unsigned char* Buttons;
        
        int HatCount;
        const unsigned char* Hats;
    };

    public value struct NativeEvent
    {
        NativeEventType Type;
        int event;
        System::IntPtr data;
    };
}
