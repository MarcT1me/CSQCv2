using SDL2;

namespace Engine.Events.QuantumEvents.Mouse;

using Input.Mouse;

public class ButtonEvent(SDL.SDL_Event e) : MouseEvent(e)
{
    public byte Button = e.button.button;
    public ButtonState State = (EventType)e.type == EventType.MouseButtonUp
        ? ButtonState.Pressed
        : ButtonState.Released;

    public byte Clicks = e.button.clicks;
    
    public int X = e.button.x;
    public int Y = e.button.y;
}