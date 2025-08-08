using OpenTK.Mathematics;
using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Mouse;

public class MouseMoveEvent(NativeMouseEvent e)
    : MouseEvent(EventType.MouseMove, e.windowID)
{
    public readonly Vector2 Pos = new(e.X, e.Y);
    
    public override string ToString()
    {
        return $"MouseEvent<{Type}>(Pos={Pos}, Window: {WindowId})";
    }
}