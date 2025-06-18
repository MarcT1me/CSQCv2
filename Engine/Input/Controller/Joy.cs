// ReSharper disable CollectionNeverQueried.Global

using System.Collections.Concurrent;
using OpenTK.Mathematics;
using SDL2;

namespace Engine.Input.Controller;

using Events.QuantumEvents;
using Events.QuantumEvents.Joy;

public class Joy(DeviceEvent deviceEvent) : IDisposable
{
    public static readonly ConcurrentDictionary<int, Joy> Joys = new();

    private readonly IntPtr _joyIndex = SDL.SDL_JoystickOpen(deviceEvent.Which);

    public int Which { get; init; } = deviceEvent.Which;

    // Axes

    private readonly short[] _axes = new short[16];
    public float GetAxis(Axis axis) => GetAxis((uint)axis);
    public float GetAxis(uint axis) => (float)_axes[axis] / short.MaxValue * 100f;

    // Buttons

    private UInt32 _pressedButtons;
    public bool IsButtonDown(JoyButtons button) => IsButtonDown((byte)button);
    public bool IsButtonDown(byte button) => (_pressedButtons & (1U << button)) != 0;

    private void SetButton(byte button, bool value)
    {
        uint mask = 1U << button;
        if (value)
            _pressedButtons |= mask;
        else
            _pressedButtons &= ~mask;
    }

    // Ball

    public readonly Vector2i[] Ball = new Vector2i[8];

    // Hats

    private readonly byte[] _hats = new byte[8];
    public byte IsHatDown(ushort hat) => _hats[hat];

    private void SetHat(ushort hat, byte value)
    {
        _hats[hat] |= value;
    }

    // Encapsulation

    public string Name => SDL.SDL_JoystickName(_joyIndex);
    public ushort Vendor => SDL.SDL_JoystickGetVendor(_joyIndex);
    public ushort Product => SDL.SDL_JoystickGetProduct(_joyIndex);
    public ushort ProductVersion => SDL.SDL_JoystickGetProductVersion(_joyIndex);
    public PowerLevel CurrentPowerLevel => (PowerLevel)SDL.SDL_JoystickCurrentPowerLevel(_joyIndex);
    public JoyType JoyType => (JoyType)SDL.SDL_JoystickGetType(_joyIndex);
    public Guid DeviceGuid => SDL.SDL_JoystickGetDeviceGUID(Which);

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
                    Joys.TryRemove(jde.Which, out _);
                }

                break;
            case AxisEvent jAxisE:
                var axisJoy = Joys[jAxisE.Which];

                axisJoy._axes[jAxisE.Axis] = jAxisE.AxisValue;
                break;
            case BallEvent jBallE:
                var ballJoy = Joys[jBallE.Which];

                ballJoy.Ball[jBallE.Ball].X = jBallE.XRel;
                ballJoy.Ball[jBallE.Ball].Y = jBallE.YRel;
                break;
            case ButtonEvent jButtonE:
                var buttonJoy = Joys[jButtonE.Which];

                buttonJoy.SetButton(jButtonE.Button, jButtonE.Type == EventType.JoyButtonDown);
                break;
            case HatEvent jHatE:
                var hatJoy = Joys[jHatE.Which];

                hatJoy.SetHat(jHatE.Hat, jHatE.HatValue);
                break;
        }
    }

    public static Joy Get(int which)
    {
        return Joys[which];
    }

    public void Dispose()
    {
        SDL.SDL_JoystickClose(_joyIndex);
    }
}