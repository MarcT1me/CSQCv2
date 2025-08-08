using Engine.Input.Keyboard;
using MirageAPI.Events;

namespace Engine.Events.QuantumEvents;

public class KeyEvent(NativeKeyEvent e)
    : WindowedQuantumEvent(e.Pressed ? EventType.KeyDown : EventType.KeyUp, e.windowID)
{
    public readonly Key Key = (Key)e.Key;
    public readonly bool Pressed = e.Pressed;
    public readonly int Scancode = e.Scancode;
    public readonly KeyMod Mods = (KeyMod)e.Mods;

    public override string ToString()
    {
        return $"KeyEvent<{Type}>(Key={Key}, Pressed={Pressed}, Scancode={Scancode}, Mods={Mods}, Window: {WindowId})";
    }
}