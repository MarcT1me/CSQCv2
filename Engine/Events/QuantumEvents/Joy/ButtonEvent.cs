using SDL2;

namespace Engine.Events.QuantumEvents.Joy;

public class ButtonEvent(SDL.SDL_Event e) : JoyEvent(e)
{
    public byte Button = e.jbutton.button;
    public byte State = e.jbutton.state;
}