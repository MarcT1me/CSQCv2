using OpenTK.Mathematics;

namespace Engine.Events.QuantumEvents.Mouse;

public class MouseMoveEvent(MirageAPI.Events.MouseEvent e)
    : MouseEvent(EventType.MouseMove, e.windowID)
{
    public readonly Vector2i Pos = e.Pos;
    public readonly Vector2i Rel = e.Rel;
    
    public override string ToString()
    {
        return $"MouseEvent<{Type}>(Pos={Pos}, Rel={Rel}, Window: {WindowId})";
    }
}