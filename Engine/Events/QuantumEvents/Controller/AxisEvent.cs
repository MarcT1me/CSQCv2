using Engine.Input.Controller;
using SDL2;

namespace Engine.Events.QuantumEvents.Controller;

public class AxisEvent(SDL.SDL_Event e) : ControllerEvent(e, e.caxis.which, e.caxis.timestamp)
{
    public Axis Axis = (Axis)e.caxis.axis;
    public short AxisValue = e.caxis.axisValue;
}