using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Mouse;

public class MouseScrollEvent(NativeMouseEvent e)
    : MouseEvent(EventType.MouseScroll, e.windowID)
{
    public readonly int Scroll = e.Button;
    
    public override string ToString()
    {
        return $"MouseEvent<{Type}>(Scroll={Scroll}, Window: {WindowId})";
    }
}