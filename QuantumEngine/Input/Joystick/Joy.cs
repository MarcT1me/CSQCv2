using System.Collections.Concurrent;

namespace Engine.Input.Joystick;

using Events.QuantumEvents;
using Events.QuantumEvents.Joystick;

public class Joy
{
    private static readonly ConcurrentDictionary<int, Joy> List = new();

    public static Joy Get(int joyId)
    {
        return List[joyId];
    }

    public bool IsButtonDown(JoyButton button) => IsButtonDown((byte)button);
    public bool IsButtonDown(byte button) => (_pressedButtons & (1U << button)) != 0;

    private void SetButton(int button, bool value)
    {
        var mask = 1U << button;
        if (value)
            _pressedButtons |= (ushort)mask;
        else
            _pressedButtons &= (ushort)~mask;
    }

    private UInt32 _pressedButtons;

    public float GetAxis(JoyAxis joyAxis) => GetAxis((uint)joyAxis);
    public float GetAxis(uint axis) => _axes[axis];

    private void SetAxis(int axis, float value) => _axes[axis] = value;

    private readonly float[] _axes = new float[16];

    internal static void Update(JoyEvent e)
    {
        if (e is JoyDeviceEvent jd)
        {
            if (jd.Connected)
                List[jd.JoystickId] = new Joy();
            else
                List.Remove(jd.JoystickId, out _);
            return;
        }

        if (!List.TryGetValue(e.JoystickId, out var joy)) return;

        joy.UpdateJoy(e);
    }

    private void UpdateJoy(JoyEvent e)
    {
        switch (e)
        {
            case JoyButtonEvent { Type: EventType.JoyButtonDown } jbd:
                SetButton(jbd.Button, true);
                break;
            case JoyButtonEvent { Type: EventType.JoyButtonUp } jbu:
                SetButton(jbu.Button, false);
                break;
            case JoyAxisEvent { Type: EventType.JoyAxisMotion } ja:
                SetAxis(ja.Axis, ja.Value);
                break;
        }
    }
}