namespace QuantumCore.Input.Keyboard;

[Flags]
public enum KeyMod
{
    Shift = 1 << 0,
    Ctrl = 1 << 1,
    Alt = 1 << 2,
    
    CapsLock = 1 << 3,
    NumLock = 1 << 4,
    ScrLock = 1 << 5
}