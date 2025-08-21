using OpenTK.Mathematics;

namespace Engine.Events.QuantumEvents.Window;

public class WinMoveEvent(MirageAPI.Events.WindowEvent e)
    : WindowedQuantumEvent(EventType.WindowMove, e.windowID)
{
    public readonly Vector2i Pos = new(e.X, e.Y);

    public override string ToString()
    {
        return $"WindowEvent<{Type}>(Pos={Pos}, Window: {WindowId})";
    }
}