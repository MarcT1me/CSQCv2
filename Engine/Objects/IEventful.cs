namespace Engine.Objects;

using Events.QuantumEvents;

public interface IEventful
{
    protected void HandleEvent(QuantumEvent e);
}