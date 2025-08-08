namespace Engine.Events.QuantumEvents.Joystick;

public class JoyEvent(EventType type, int joystickId) 
    : QuantumEvent(type)
{
    public readonly int JoystickId = joystickId;
    
    public override string ToString()
    {
        return $"JoyEvent<{Type}>(Joy={JoystickId})";
    }
}