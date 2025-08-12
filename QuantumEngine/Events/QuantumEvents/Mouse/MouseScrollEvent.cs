using OpenTK.Mathematics;
using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Mouse;

public class MouseScrollEvent(NativeMouseEvent e)
    : MouseEvent(EventType.MouseScroll, e.windowID)
{
    public readonly Vector2 Rel = e.Rel;
    
    public override string ToString()
    {
        return $"MouseEvent<{Type}>(Rel={Rel}, Window: {WindowId})";
    }
}