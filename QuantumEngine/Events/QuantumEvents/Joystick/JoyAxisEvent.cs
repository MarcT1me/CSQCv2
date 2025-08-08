namespace Engine.Events.QuantumEvents.Joystick;

public class JoyAxisEvent(int joystickId, int axis, float value) 
    : JoyEvent(EventType.JoyAxisMotion, joystickId)
{
    public readonly int Axis = axis;
    public readonly float Value = value;
    
    public override string ToString()
    {
        return $"JoyEvent<{Type}>(Axis={Axis}, Value={Value}, Joy: {JoystickId})";
    }
}