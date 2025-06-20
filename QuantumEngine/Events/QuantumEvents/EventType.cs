namespace Engine.Events.QuantumEvents;

public enum EventType : uint
{
    Simple,

    KeyDown,
    KeyUp,

    MouseButtonDown,
    MouseButtonUp,
    MouseMove,
    MouseRel,

    WindowFocusGained,
    WindowFocusLost,
    WindowMaximize,
    WindowMinimize,
    WindowRestore,
    WindowIconify,
    WindowResize,
    WindoweMove,
    WindowClose,

    Char,
    Drop,
    Joystick,
    Monitor,
    
    Quit,
}