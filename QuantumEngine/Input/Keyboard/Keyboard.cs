using System.Collections.Concurrent;

namespace QuantumCore.Input.Keyboard;

using Events.QuantumEvents;
using Graphic.Window;

public class Keyboard(IntPtr windowHandle)
{
    internal static readonly ConcurrentDictionary<IntPtr, Keyboard> List = new();

    // static (last updated window -> PyGame and SDL format)

    public static bool GetKey(Key key) => GetKey((int)key);
    public static bool GetKey(int key) => LastActiveWindowId is not 0 && Get(LastActiveWindowId).PressedKeys[key];

    public static Keyboard Get(IntPtr windowId)
    {
        return List[windowId];
    }

    public readonly IntPtr WindowHandle = windowHandle;

    public static IntPtr LastActiveWindowId { get; private set; }

    // keys

    public readonly bool[] PressedKeys = new bool[512];

    // updating

    internal static void Update(KeyEvent e)
    {
        if (!List.TryGetValue(e.WindowId, out var keyboard)) return;

        keyboard.UpdateKey(e);
        LastActiveWindowId = e.WindowId;
    }

    private void UpdateKey(KeyEvent e)
    {
        PressedKeys[(int)e.Key] = e.Type == EventType.KeyDown;
    }

    // registration

    internal static void RegisterWindow(Window window)
    {
        List[window.Handle] = new Keyboard(window.Handle);
    }

    internal static void UnregisterWindow(Window window)
    {
        List.Remove(window.Handle, out _);
    }

    public override string ToString()
    {
        return $"Keyboard<{WindowHandle}>(LastActiveWindowId: {LastActiveWindowId})";
    }
}