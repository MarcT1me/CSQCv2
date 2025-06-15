using SDL2;

namespace Engine.Events.QuantumEvents.Mouse;

public class WheelEvent(SDL.SDL_Event e) : MouseEvent(e)
{
    public int X = e.wheel.x;
    public int Y = e.wheel.y;
    public uint Direction = e.wheel.direction;
}