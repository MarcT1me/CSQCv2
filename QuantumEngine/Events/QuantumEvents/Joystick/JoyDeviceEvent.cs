namespace QuantumCore.Events.QuantumEvents.Joystick;

public class JoyDeviceEvent(int joystickId, bool connected)
    : JoyEvent(connected ? EventType.JoyDeviceConnect : EventType.JoyDeviceDisconnect, joystickId)
{
    public readonly bool Connected = connected;
    
    public override string ToString()
    {
        return $"JoyEvent<{Type}>(Connected={Connected}, Joy: {JoystickId})";
    }
}