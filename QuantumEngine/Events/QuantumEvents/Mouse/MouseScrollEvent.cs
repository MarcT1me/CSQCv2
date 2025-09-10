namespace QuantumCore.Events.QuantumEvents.Mouse;

public class MouseScrollEvent(MirageAPI.Events.MouseEvent e)
    : MouseEvent(EventType.MouseScroll, e.windowID)
{
    public readonly int Scroll = e.Button;
    
    public override string ToString()
    {
        return $"MouseEvent<{Type}>(Scroll={Scroll}, Window: {WindowId})";
    }
}