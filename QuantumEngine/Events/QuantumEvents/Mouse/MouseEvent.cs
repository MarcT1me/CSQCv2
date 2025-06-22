namespace Engine.Events.QuantumEvents.Mouse;

public class MouseEvent(EventType type, IntPtr windowId) 
    : WindowedQuantumEvent(type, windowId);