namespace QuantumCore.Events.QuantumEvents;

public enum EventType : uint
{
    Unknown,

    KeyDown,
    KeyUp,

    MouseButtonDown,
    MouseButtonUp,
    MouseMove,
    MouseEnter,
    MouseLeave,
    MouseScroll,
    
    WindowFocusGained,
    WindowFocusLost,
    WindowResize,
    WindowMove,
    WindowCreate,
    WindowClose,
    WindowDestroy,
    
    Char,
    Drop,
    
    JoyDeviceConnect,
    JoyDeviceDisconnect,
    JoyAxisMotion,
    JoyButtonDown,
    JoyButtonUp,
    JoyHatDown,
    JoyHatUp,

    Monitor,
    
    Quit,
}