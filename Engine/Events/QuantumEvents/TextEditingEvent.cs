using SDL2;

namespace Engine.Events.QuantumEvents;

public class TextEditingEvent(SDL.SDL_Event e) : WindowedQuantumEvent(e, e.edit.timestamp, e.edit.windowID)
{
    public readonly unsafe byte* Text = e.edit.text;
    public int Start = e.edit.start;
    public int Length = e.edit.length;
}