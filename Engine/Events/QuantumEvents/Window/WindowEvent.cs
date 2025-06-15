using SDL2;

namespace Engine.Events.QuantumEvents.Window;

public class WindowEvent(SDL.SDL_Event e) : WindowedQuantumEvent(e, e.window.timestamp, e.window.windowID)
{
    public int Data1 = e.window.data1;
    public int Data2 = e.window.data2;
}