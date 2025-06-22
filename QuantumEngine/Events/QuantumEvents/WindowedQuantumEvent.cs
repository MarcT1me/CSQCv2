namespace Engine.Events.QuantumEvents;

public class WindowedQuantumEvent(EventType type, IntPtr windowId)
    : QuantumEvent(type)
{
    public readonly IntPtr WindowId = windowId;
}