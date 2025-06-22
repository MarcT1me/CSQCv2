namespace Engine.Events.QuantumEvents.Joystick;

public class JoyButtonEvent(int joystickId, int button, bool pressed)
    : JoyEvent(pressed ? EventType.JoyButtonDown : EventType.JoyButtonUp, joystickId)
{
    public int Button { get; } = button;
    public bool Pressed { get; } = pressed;
}