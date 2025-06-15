using Engine.Events.QuantumEvents;

namespace Engine.Input.Keyboard;

public static class Keyboard
{
    private static readonly bool[] PressedKeys = new bool[512];
    
    public static bool IsKeyDown(KeyCode key) => PressedKeys[(int)key];
    
    public static void Update(KeyboardEvent ke)
    {
        PressedKeys[(int)ke.KeySym.Scancode] = ke.State == KeyState.Down;
    }
}