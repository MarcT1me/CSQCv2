using System.Collections.Concurrent;

namespace Engine.Input.Keyboard;

using Events.QuantumEvents;
using Graphic.Window;

public class Keyboard
{
    private static readonly ConcurrentDictionary<IntPtr, Keyboard> Windows = new();

    public static bool GetKey(int button) => Get(LastActiveWindowId).IsKeyDown(button);

    public static Keyboard Get(IntPtr windowId)
    {
        return Windows[windowId];
    }

    public static IntPtr LastActiveWindowId { get; private set; }

    public bool IsKeyDown(int key) => _pressedKeys[key];

    private readonly bool[] _pressedKeys = new bool[512];

    internal static void Update(KeyEvent e)
    {
        if (!Windows.TryGetValue(e.WindowId, out var keyboard)) return;

        keyboard.UpdateKey(e);
        LastActiveWindowId = e.WindowId;
    }

    private void UpdateKey(KeyEvent e)
    {
        _pressedKeys[e.Key] = e.Type == EventType.KeyDown;
    }

    public static void RegisterWindow(Window window)
    {
        Windows[window.Handle] = new Keyboard();
    }

    public static void UnregisterWindow(Window window)
    {
        Windows.Remove(window.Handle, out _);
    }
}