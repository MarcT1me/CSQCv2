using SDL2;

namespace Engine.Events.QuantumEvents.Mouse;

public class MotionEvent(SDL.SDL_Event e) : MouseEvent(e)
{
    public int X = e.motion.x;
    public int Y = e.motion.y;
    public int XRel = e.motion.xrel;
    public int YRel = e.motion.yrel;
}