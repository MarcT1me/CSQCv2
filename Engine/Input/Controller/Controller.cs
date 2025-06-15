namespace Engine.Input.Controller;

using Data.Collections;
using Data.Meta;
using Events.QuantumEvents;
using Events.QuantumEvents.Controller;

public class Controller(DeviceEvent deviceEvent)
{
    public static readonly WritableTale<Controller> Controllers = new(new MetaData("ControllersList"));

    public int Which { get; init; } = deviceEvent.Which;

    public readonly short[] Axes = new short[16];

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

    public static void Update(ControllerEvent e)
    {
        switch (e)
        {
            case DeviceEvent cde:
                if (e.Type == EventType.ControllerDeviceAdded)
                {
                    Controllers[cde.Which] = new Controller(cde);
                }
                else if (e.Type == EventType.ControllerDeviceRremoved)
                {
                    Controllers.Pop(cde.Which);
                }

                break;
            case AxisEvent cAxisE:
                var axisController = Controllers[cAxisE.Which];
                if (axisController == null) break;

                axisController.Axes[cAxisE.Axis] = cAxisE.AxisValue;
                break;
            case ButtonEvent cButtonE:
                var buttonController = Controllers[cButtonE.Which];
                if (buttonController == null) break;

                buttonController.SetButton(cButtonE.Button, cButtonE.Type == EventType.JoyButtonDown);
                break;
        }
    }
}