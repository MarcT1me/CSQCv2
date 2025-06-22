using System.Collections.Concurrent;
using OpenTK.Mathematics;

namespace Engine.Input.Mouse;

using Events.QuantumEvents;
using Events.QuantumEvents.Mouse;
using Graphic.Window;

public class Mouse(IntPtr windowHandle)
{
    internal static readonly ConcurrentDictionary<IntPtr, Mouse> List = new();

    // static (last updated window -> PyGame and SDL format)
    
    public static bool GetButton(MouseButton button) => Get(LastActiveWindowId).IsButtonDown(button);
    public static bool GetButton(byte button) => Get(LastActiveWindowId).IsButtonDown(button);

    public static Vector2i GetPos() => Get(LastActiveWindowId).Pos;
    public static Vector2i GetRell() => Get(LastActiveWindowId).ScrollPos;

    public static Mouse Get(IntPtr windowId)
    {
        return List[windowId];
    }
    
    public readonly IntPtr WindowHandle = windowHandle;

    public static IntPtr LastActiveWindowId { get; private set; }

    // buttons
    
    public bool IsButtonDown(MouseButton button) => IsButtonDown((byte)button);
    public bool IsButtonDown(byte button) => (_pressedButtons & (1U << button)) != 0;

    private void SetButton(int button, bool value)
    {
        var mask = 1U << button;
        if (value)
            _pressedButtons |= (ushort)mask;
        else
            _pressedButtons &= (ushort)~mask;
    }

    private UInt16 _pressedButtons;
    
    // vec sroll and pos
    
    public Vector2i ScrollPos { get; private set; } = Vector2i.Zero;
    public Vector2i Pos { get; private set; } = Vector2i.Zero;

    // updating
    
    internal static void Update(MouseEvent e)
    {
        if (!List.TryGetValue(e.WindowId, out var mouse)) return;
        
        mouse.UpdateMouse(e);
        LastActiveWindowId = e.WindowId;
    }

    private void UpdateMouse(MouseEvent e)
    {
        switch (e)
        {
            case MouseButtonEvent { Type: EventType.MouseButtonDown } mbd:
                SetButton(mbd.Button, true);
                break;
            case MouseButtonEvent { Type: EventType.MouseButtonUp } mbu:
                SetButton(mbu.Button, false);
                break;
            case MouseScrollEvent { Type: EventType.MouseScroll } ms:
                ScrollPos = new((int)ms.X, (int)ms.Y);
                break;
            case MouseMoveEvent { Type: EventType.MouseMove } mm:
                Pos = new((int)mm.X, (int)mm.Y);
                break;
        }
    }

    // registration
    
    public static void RegisterWindow(Window window)
    {
        List[window.Handle] = new Mouse(window.Handle);
    }

    public static void UnregisterWindow(Window window)
    {
        List.Remove(window.Handle, out _);
    }
}