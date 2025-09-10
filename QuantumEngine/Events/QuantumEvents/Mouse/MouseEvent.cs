namespace QuantumCore.Events.QuantumEvents.Mouse;

public class MouseEvent(EventType type, IntPtr windowId) : WindowedQuantumEvent(type, windowId)
{
    public override string ToString()
    {
        return $"MouseEvent<{Type}>(Window: {WindowId})";
    }
}