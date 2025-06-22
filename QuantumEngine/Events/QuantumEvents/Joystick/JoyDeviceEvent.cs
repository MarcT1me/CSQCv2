using MirageAPI.Events;

namespace Engine.Events.QuantumEvents.Joystick;

public class JoyDeviceEvent(NativeJoystickEvent e)
    : JoyEvent(EventType.JoyDevice, e.JoystickID)
{
    public bool Connected { get; } = e.Connected;
}