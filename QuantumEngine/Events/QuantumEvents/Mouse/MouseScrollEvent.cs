using OpenTK.Mathematics;
using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Mouse;

public class MouseScrollEvent(NativeMouseEvent e)
    : MouseEvent(EventType.MouseScroll, e.windowID)
{
    public readonly Vector2 Scroll = new(e.X, e.Y);
    
    public override string ToString()
    {
        return $"MouseEvent<{Type}>(Scroll={Scroll}, Window: {WindowId})";
    }
}