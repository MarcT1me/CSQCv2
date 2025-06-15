using SDL2;

namespace Engine.Events.QuantumEvents.Controller;

public class AxisEvent(SDL.SDL_Event e) : ControllerEvent(e)
{
    public byte Axis = e.caxis.axis;
    public short AxisValue = e.caxis.axisValue;
}