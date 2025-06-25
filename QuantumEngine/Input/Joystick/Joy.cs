using System.Collections.Concurrent;
using MirageAPI.Events;

namespace Engine.Input.Joystick;

using Events.QuantumEvents;
using Events.QuantumEvents.Joystick;

public class Joy(int jid)
{
    internal static readonly ConcurrentDictionary<int, Joy> List = new();

    public static Joy Get(int joyId)
    {
        return List[joyId];
    }

    public readonly int Jid = jid;

    // buttons

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

    // hats

    public bool IsHatDown(byte hat) => (_pressedHats & (1U << hat)) != 0;

    private void SetHat(int hat, bool value)
    {
        var mask = 1U << hat;
        if (value)
            _pressedHats |= (ushort)mask;
        else
            _pressedHats &= (ushort)~mask;
    }

    private UInt32 _pressedHats;

    // axes

    public float GetAxis(JoyAxis joyAxis) => GetAxis((uint)joyAxis);
    public float GetAxis(uint axis) => _axes[axis];

    private void SetAxis(int axis, float value) => _axes[axis] = value;

    private readonly float[] _axes = new float[16];

    // updating

    internal static void Update(JoyEvent e)
    {
        if (e is JoyDeviceEvent jd)
        {
            if (jd.Connected)
                List[jd.JoystickId] = new Joy(e.JoystickId);
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
            case JoyHatEvent { Type: EventType.JoyHatDown } jhd:
                SetHat(jhd.Hat, true);
                break;
            case JoyHatEvent { Type: EventType.JoyHatUp } jhu:
                SetHat(jhu.Hat, false);
                break;
            case JoyAxisEvent { Type: EventType.JoyAxisMotion } ja:
                SetAxis(ja.Axis, ja.Value);
                break;
        }
    }
}