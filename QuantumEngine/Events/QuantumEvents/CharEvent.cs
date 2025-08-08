using MirageAPI.Events;

namespace Engine.Events.QuantumEvents;

public class CharEvent(NativeCharEvent e) 
    : WindowedQuantumEvent(EventType.Char, e.windowID)
{
    public readonly uint CodePoint = e.codepoint;

    public override string ToString()
    {
        return $"CharEvent<{Type}>(CodePoint={CodePoint}, Window: {WindowId})";
    }
}