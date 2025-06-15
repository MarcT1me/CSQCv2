using SDL2;

namespace Engine.Events.QuantumEvents.Joy;

public class JoyEvent(SDL.SDL_Event e) : QuantumEvent(e, e.jaxis.timestamp)
{
    public int Which = e.jaxis.which;
}