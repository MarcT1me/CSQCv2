using SDL2;

namespace Engine.Events.QuantumEvents;

using Input.Keyboard;

public class KeyboardEvent(SDL.SDL_Event e) : WindowedQuantumEvent(e, e.key.timestamp, e.key.windowID)
{
    public KeyState State = (EventType)e.type == EventType.KeyUp ? KeyState.Up : KeyState.Down;
    public byte Repeat = e.key.repeat;
    public KeySym KeySym = new(e.key.keysym);
}