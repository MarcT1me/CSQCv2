namespace QuantumCore.Events.QuantumEvents.Joystick;

public class JoyButtonEvent(int joystickId, int button, bool pressed)
    : JoyEvent(pressed ? EventType.JoyButtonDown : EventType.JoyButtonUp, joystickId)
{
    public readonly int Button = button;
    public readonly bool Pressed = pressed;

    public override string ToString()
    {
        return $"JoyEvent<{Type}>(Button={Button}, Pressed={Pressed}, Joy: {JoystickId})";
    }
}