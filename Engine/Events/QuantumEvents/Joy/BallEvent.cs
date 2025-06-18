using SDL2;

namespace Engine.Events.QuantumEvents.Joy;

public class BallEvent(SDL.SDL_Event e) : JoyEvent(e, e.jball.which, e.jball.timestamp)
{
    public byte Ball = e.jball.ball;
    public short XRel = e.jball.xrel;
    public short YRel = e.jball.yrel;
}