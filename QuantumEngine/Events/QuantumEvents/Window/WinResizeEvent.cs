using OpenTK.Mathematics;

namespace Engine.Events.QuantumEvents.Window;

public class WinResizeEvent(MirageAPI.Events.WindowEvent e)
    : WindowedQuantumEvent(EventType.WindowResize, e.windowID)
{
    public readonly Vector2i Size = new(e.X, e.Y);

    public override string ToString()
    {
        return $"WindowEvent<{Type}>(Size={Size}, Window: {WindowId})";
    }
}