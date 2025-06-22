namespace Engine.Events.QuantumEvents.Joystick;

public class JoyDeviceEvent(int joystickId, bool connected)
    : JoyEvent(EventType.JoyDevice, joystickId)
{
    public bool Connected { get; } = connected;
}