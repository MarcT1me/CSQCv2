namespace Engine.Events.QuantumEvents.Joystick;

public class JoyEvent(EventType type, int joystickId) 
    : QuantumEvent(type)
{
    public int JoystickId { get; } = joystickId;
}