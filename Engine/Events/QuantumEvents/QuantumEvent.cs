using SDL2;

namespace Engine.Events.QuantumEvents;

public class QuantumEvent(SDL.SDL_Event e, uint timestamp)
{
    public EventType Type = (EventType)e.type;
    public uint Timestamp = timestamp;
}