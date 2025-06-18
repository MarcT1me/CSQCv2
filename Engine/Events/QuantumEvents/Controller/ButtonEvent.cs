using SDL2;

namespace Engine.Events.QuantumEvents.Controller;

public class ButtonEvent(SDL.SDL_Event e) : ControllerEvent(e, e.cbutton.which, e.cbutton.timestamp)
{
    public byte Button = e.cbutton.button;
    public byte State = e.cbutton.state;
}