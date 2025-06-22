using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Mouse;

public class MouseMoveEvent(NativeMouseEvent e)
    : MouseEvent(EventType.MouseMove, e.windowID)
{
    public double X = e.X;
    public double Y = e.Y;
}