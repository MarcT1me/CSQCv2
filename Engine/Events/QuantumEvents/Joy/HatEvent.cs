using SDL2;

namespace Engine.Events.QuantumEvents.Joy;

public class HatEvent(SDL.SDL_Event e) : JoyEvent(e)
{
    public byte Hat = e.jhat.hat;
    public byte HatValue = e.jhat.hatValue;
}