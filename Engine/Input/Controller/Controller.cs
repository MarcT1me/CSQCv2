using System.Collections.Concurrent;
using SDL2;

namespace Engine.Input.Controller;

using Events.QuantumEvents;
using Events.QuantumEvents.Controller;

public class Controller(DeviceEvent deviceEvent) : IDisposable
{
    public static readonly ConcurrentDictionary<int, Controller> Controllers = new();

    private readonly IntPtr _controllerIndex = SDL.SDL_GameControllerOpen(deviceEvent.Which);

    public int Which { get; init; } = deviceEvent.Which;

    // Axes

    private readonly short[] _axes = new short[6];
    public float GetAxis(Axis axis) => (float)_axes[(uint)axis] / short.MaxValue * 100f;

    // Buttons

    private UInt32 _pressedButtons;
    public bool IsButtonDown(ControllerButton button) => (_pressedButtons & (1U << (byte)button)) != 0;

    private void SetButton(ControllerButton button, bool value)
    {
        uint mask = 1U << (byte)button;
        if (value)
            _pressedButtons |= mask;
        else
            _pressedButtons &= ~mask;
    }

    // Encapsulation

    public string Name => SDL.SDL_GameControllerName(_controllerIndex);
    public ushort Vendor => SDL.SDL_GameControllerGetVendor(_controllerIndex);
    public ushort Product => SDL.SDL_GameControllerGetProduct(_controllerIndex);
    public ushort ProductVersion => SDL.SDL_GameControllerGetProductVersion(_controllerIndex);

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
                    Controllers.TryRemove(cde.Which, out _);
                }

                break;
            case AxisEvent cAxisE:
                var axisController = Controllers[cAxisE.Which];

                axisController._axes[(int)cAxisE.Axis] = cAxisE.AxisValue;
                break;
            case ButtonEvent cButtonE:
                var buttonController = Controllers[cButtonE.Which];

                buttonController.SetButton(
                    (ControllerButton)cButtonE.Button,
                    cButtonE.Type == EventType.ControllerButtonDown
                );
                break;
        }
    }

    public static Controller Get(int which)
    {
        return Controllers[which];
    }

    public void Dispose()
    {
        SDL.SDL_GameControllerClose(_controllerIndex);
    }
}