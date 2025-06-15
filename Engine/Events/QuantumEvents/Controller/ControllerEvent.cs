using SDL2;

namespace Engine.Events.QuantumEvents.Controller;

public class ControllerEvent(SDL.SDL_Event e) : QuantumEvent(e, e.caxis.timestamp)
{
    public int Which = e.caxis.which;
}