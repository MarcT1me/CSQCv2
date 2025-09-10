namespace QuantumCore.Events.QuantumEvents.Joystick;

public class JoyHatEvent(int joystickId, int button, bool pressed)
    : JoyEvent(pressed ? EventType.JoyHatDown : EventType.JoyHatUp, joystickId)
{
    public readonly int Hat = button;
    public readonly bool Pressed = pressed;
    
    public override string ToString()
    {
        return $"JoyEvent<{Type}>(Hat={Hat}, Pressed={Pressed}, Joy: {JoystickId})";
    }
}