#pragma once

using namespace System;
using namespace OpenTK::Mathematics;

namespace MirageAPI::Events
{
    public ref struct EventParams
    {
        unsigned short lowW;
        unsigned short highW;
        unsigned short lowL;
        unsigned short highL;

        WPARAM wParam;
        LPARAM lParam;

    internal:
        EventParams(WPARAM wParam, LPARAM lParam) : wParam(wParam), lParam(lParam)
        {
            lowW = LOWORD(wParam);
            highW = HIWORD(wParam);
            lowL = LOWORD(lParam);
            highL = HIWORD(lParam);
        }
    };

    public enum class EventType
    {
        MouseButton,
        MouseMove, MouseScroll,
        MouseLeave, MouseEnter,

        WindowFocus, WindowVisibility,
        WindowDisplayChange,
        WindowResize, WindowMove,
        WindowCreate, WindowClose, WindowDestroy
    };

    public ref class Event
    {
    public:
        EventType Type;
        IntPtr windowID;
    };

    public ref class KeyEvent : Event
    {
    public:
        int Key;
        bool Pressed;

        int Scancode;
        int Mods;
    };

    public ref class MouseEvent : Event
    {
    public:
        int Button;
        bool Pressed;

        Vector2i Pos;
        Vector2i Rel;

        int Mode;
    };

    public ref class WindowEvent : Event
    {
    public:
        bool Flag;
        int State;

        int X;
        int Y;
    };

    public ref class DropEvent : Event
    {
    public:
        int count;
        CSList<String^>^ paths;
    };

    public ref struct JoystickState
    {
        int JoystickID;

        int AxesCount;
        const float* Axes;

        int ButtonCount;
        const unsigned char* Buttons;

        int HatCount;
        const unsigned char* Hats;
    };
}
