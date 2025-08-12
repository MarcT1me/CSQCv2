using OpenTK.Mathematics;
using MirageAPI.Events;
using WinRT;

namespace Engine.Events.QuantumEvents.Mouse;

public class MouseMoveEvent(NativeMouseEvent e)
    : MouseEvent(EventType.MouseMove, e.windowID)
{
    public readonly Vector2i Pos = e.Pos;
    public readonly Vector2i Rel = e.Rel;
    
    public override string ToString()
    {
        return $"MouseEvent<{Type}>(Pos={Pos}, Rel={Rel}, Window: {WindowId})";
    }
}