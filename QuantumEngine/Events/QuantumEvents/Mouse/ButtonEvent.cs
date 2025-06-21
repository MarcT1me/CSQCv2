using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Mouse;

public class ButtonEvent(NativeMouseEvent e)
    : MouseEvent(e.Action == 1 ? EventType.MouseButtonDown : EventType.MouseButtonUp, e.windowID)
{
    public int Button = e.Button;
    public int Mode = e.Mode;
}