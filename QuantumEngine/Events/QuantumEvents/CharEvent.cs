using MirageAPI.Events;

namespace Engine.Events.QuantumEvents;

public class CharEvent(NativeCharEvent e) 
    : WindowedQuantumEvent(EventType.Char, e.windowID)
{
    public uint CodePoint = e.codepoint;
}