using SDL2;

namespace Engine.Events.QuantumEvents;

public class WindowedQuantumEvent(SDL.SDL_Event e, uint timestamp, uint windowId) : QuantumEvent(e, timestamp)
{
    public uint WindowId = windowId;
}