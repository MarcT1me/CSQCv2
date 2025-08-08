namespace Engine.Input.Mouse;

[Flags]
public enum ButtonMod
{
    Shift = 1 << 0,
    Ctrl = 1 << 1,
    Alt = 1 << 2
}