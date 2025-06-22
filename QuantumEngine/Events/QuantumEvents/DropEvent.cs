using System.Runtime.InteropServices;
using MirageAPI.Events;

namespace Engine.Events.QuantumEvents;

public class DropEvent 
    : WindowedQuantumEvent
{
    public string? Paths;

    public DropEvent(NativeDropEvent e) : base(EventType.Drop, e.windowID)
    {
        unsafe
        {
            Paths = Marshal.PtrToStringAnsi((IntPtr)e.paths);
        }
    }
}