using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Window;

public class IconifyEvent(NativeWindowEvent e)
    : WindowedQuantumEvent(EventType.WindowIconify, e.windowID)
{
    public readonly int Iconify = e.X;
}