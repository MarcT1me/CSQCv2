namespace Engine.Events.QuantumEvents;

public enum EventType : uint
{
    Unknown,

    KeyDown,
    KeyUp,

    MouseButtonDown,
    MouseButtonUp,
    MouseMove,
    MouseScroll,
    
    WindowFocusGained,
    WindowFocusLost,
    WindowMaximize,
    WindowMinimize,
    WindowRestore,
    WindowIconify,
    WindowResize,
    WindowMove,
    WindowClose,
    
    Char,
    Drop,
    
    JoyDevice,
    JoyAxisMotion,
    JoyButtonDown,
    JoyButtonUp,
    JoyHatDown,
    JoyHatUp,

    Monitor,
    
    Quit,
}