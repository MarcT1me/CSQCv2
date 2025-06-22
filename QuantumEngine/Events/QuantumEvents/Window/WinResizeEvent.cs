using MirageAPI.Events;
using OpenTK.Mathematics;

namespace Engine.Events.QuantumEvents.Window;

public class WinResizeEvent(NativeWindowEvent e)
    : WindowedQuantumEvent(EventType.WindowResize, e.windowID)
{
    public Vector2i Size = new(e.X, e.Y);
}