using SDL2;

namespace Engine.Events.QuantumEvents.Controller;

public class ControllerEvent(SDL.SDL_Event e, int which, uint timestamp) : QuantumEvent(e, timestamp)
{
    public int Which = which;
}