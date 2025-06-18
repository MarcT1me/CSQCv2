using SDL2;

namespace Engine.Events.QuantumEvents.Joy;

public class JoyEvent(SDL.SDL_Event e, int which, uint timestamp) : QuantumEvent(e, timestamp)
{
    public int Which = which;
}