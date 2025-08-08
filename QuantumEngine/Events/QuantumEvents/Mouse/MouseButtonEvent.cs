using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Mouse;

public class MouseButtonEvent(NativeMouseEvent e)
    : MouseEvent(e.Pressed ? EventType.MouseButtonDown : EventType.MouseButtonUp, e.windowID)
{
    public readonly int Button = e.Button;
    public readonly bool Pressed = e.Pressed;
    public readonly int Mode = e.Mode;
    
    public override string ToString()
    {
        return $"MouseEvent<{Type}>(Button={Button}, Pressed={Pressed}, Mode={Mode}, Window:{WindowId})";
    }
}