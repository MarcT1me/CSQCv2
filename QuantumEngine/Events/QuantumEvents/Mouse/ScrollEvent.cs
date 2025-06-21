using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Mouse;

public class ScrollEvent(NativeMouseEvent e)
    : MouseEvent(EventType.MouseScroll, e.windowID)
{
    public double X = e.X;
    public double Y = e.Y;
}