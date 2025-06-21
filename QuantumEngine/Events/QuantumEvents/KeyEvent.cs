using MirageAPI.Events;

namespace Engine.Events.QuantumEvents;

public class KeyEvent(NativeKeyEvent e)
    : WindowedQuantumEvent(e.Action == 1 ? EventType.KeyDown : EventType.KeyUp, e.windowID)
{
    public int Key = e.Key;
    public int Scancode = e.Scancode;
    public int Mods = e.Mods;
}