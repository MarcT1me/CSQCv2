using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Window;

public class WinIconifyEvent(NativeWindowEvent e)
    : WindowedQuantumEvent(EventType.WindowIconify, e.windowID)
{
    public readonly int Iconify = e.X;

    public override string ToString()
    {
        return $"WindowEvent<{Type}>(Iconify={Iconify}, Window: {WindowId})";
    }
}