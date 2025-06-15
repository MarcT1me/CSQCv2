using SDL2;

namespace Engine.Events.QuantumEvents;

public class DropEvent(SDL.SDL_Event e) : QuantumEvent(e, e.drop.timestamp)
{
    public IntPtr File = e.drop.file;
}