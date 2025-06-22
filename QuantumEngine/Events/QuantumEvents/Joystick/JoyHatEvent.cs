namespace Engine.Events.QuantumEvents.Joystick;

public class JoyHatEvent(int joystickId, int button, bool pressed)
    : JoyEvent(pressed ? EventType.JoyHatDown : EventType.JoyHatUp, joystickId)
{
    public int Hat { get; } = button;
    public bool Pressed { get; } = pressed;
}