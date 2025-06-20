namespace Engine.Events.QuantumEvents;

public class QuantumEvent(EventType type)
{
    public readonly EventType Type = type;
}