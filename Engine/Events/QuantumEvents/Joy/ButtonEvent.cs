using SDL2;

namespace Engine.Events.QuantumEvents.Joy;

public class ButtonEvent(SDL.SDL_Event e) : JoyEvent(e, e.jbutton.which, e.jbutton.timestamp)
{
    public byte Button = e.jbutton.button;
    public byte State = e.jbutton.state;
}