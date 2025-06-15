using SDL2;

namespace Engine.Events.QuantumEvents.Joy;

public class AxisEvent(SDL.SDL_Event e) : JoyEvent(e)
{
    public byte Axis = e.jaxis.axis;
    public short AxisValue = e.jaxis.axisValue;
}