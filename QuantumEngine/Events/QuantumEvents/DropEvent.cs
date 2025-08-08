using System.Runtime.InteropServices;
using MirageAPI.Events;

namespace Engine.Events.QuantumEvents;

public class DropEvent 
    : WindowedQuantumEvent
{
    public readonly string? Paths;

    public DropEvent(NativeDropEvent e) : base(EventType.Drop, e.windowID)
    {
        unsafe
        {
            Paths = Marshal.PtrToStringAnsi((IntPtr)e.paths);
        }
    }

    public override string ToString()
    {
        return $"DropEvent<{Type}>(Paths={Paths}, Window: {WindowId})";
    }
}