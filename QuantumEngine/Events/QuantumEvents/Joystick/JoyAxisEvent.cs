namespace Engine.Events.QuantumEvents.Joystick;

public class JoyAxisEvent(int joystickId, int axis, float value) 
    : JoyEvent(EventType.JoyAxisMotion, joystickId)
{
    public int Axis { get; } = axis;
    public float Value { get; } = value;
}