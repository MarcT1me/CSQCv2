using MirageAPI.Events;
using OpenTK.Mathematics;

namespace Engine.Events.QuantumEvents.Window;

public class WinMoveEvent(NativeWindowEvent e)
    : WindowedQuantumEvent(EventType.WindowMove, e.windowID)
{
    public Vector2i Pos = new(e.X, e.Y);
}