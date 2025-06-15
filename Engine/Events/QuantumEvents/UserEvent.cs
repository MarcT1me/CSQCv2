using SDL2;

namespace Engine.Events.QuantumEvents;

public class UserEvent(SDL.SDL_Event e) : WindowedQuantumEvent(e, e.user.timestamp, e.user.windowID)
{
    public int Code = e.user.code;
    public IntPtr Data1 = e.user.data1;
    public IntPtr Data2 = e.user.data2;
}