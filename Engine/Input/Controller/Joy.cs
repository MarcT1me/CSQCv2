// ReSharper disable CollectionNeverQueried.Global

using OpenTK.Mathematics;

namespace Engine.Input.Controller;

using Data.Collections;
using Data.Meta;
using Events.QuantumEvents;
using Events.QuantumEvents.Joy;

public sealed class Joy(DeviceEvent deviceEvent)
{
    public static readonly WritableTale<Joy> Joys = new(new MetaData("JoysList"));

    public int Which { get; init; } = deviceEvent.Which;

    public readonly short[] Axes = new short[16];

    public readonly Vector2i[] Ball = new Vector2i[8];

    private UInt32 _pressedButtons;
    public bool IsButtonDown(byte button) => (_pressedButtons & (1U << button)) != 0;

    private void SetButton(byte button, bool value)
    {
        uint mask = 1U << button;
        if (value)
            _pressedButtons |= mask;
        else
            _pressedButtons &= ~mask;
    }

    public readonly byte[] Hats = new byte[16];

    public static void Update(JoyEvent e)
    {
        switch (e)
        {
            case DeviceEvent jde:
                if (e.Type == EventType.JoyDeviceAdded)
                {
                    Joys[jde.Which] = new Joy(jde);
                }
                else if (e.Type == EventType.JoyDeviceRemoved)
                {
                    Joys.Pop(jde.Which);
                }
                break;
            case AxisEvent jAxisE:
                var axisJoy = Joys[jAxisE.Which];
                if (axisJoy == null) break;

                axisJoy.Axes[jAxisE.Axis] = jAxisE.AxisValue;
                break;
            case BallEvent jBallE:
                var ballJoy = Joys[jBallE.Which];
                if (ballJoy == null) break;

                ballJoy.Ball[jBallE.Ball].X = jBallE.XRel;
                ballJoy.Ball[jBallE.Ball].Y = jBallE.YRel;
                break;
            case ButtonEvent jButtonE:
                var buttonJoy = Joys[jButtonE.Which];
                if (buttonJoy == null) break;

                buttonJoy.SetButton(jButtonE.Button, jButtonE.Type == EventType.JoyButtonDown);
                break;
            case HatEvent jHatE:
                var hatJoy = Joys[jHatE.Which];
                if (hatJoy == null) break;

                hatJoy.Hats[jHatE.Hat] = jHatE.HatValue;
                break;
        }
    }
}