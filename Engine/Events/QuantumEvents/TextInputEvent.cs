using SDL2;

namespace Engine.Events.QuantumEvents;

public class TextInputEvent(SDL.SDL_Event e) : WindowedQuantumEvent(e, e.edit.timestamp, e.edit.windowID)
{
    public readonly unsafe byte* Text = e.edit.text;
}