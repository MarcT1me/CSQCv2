namespace Engine.Input.Mouse;

using Events.QuantumEvents.Mouse;

public static class Mouse
{
    private static readonly bool[] PressedButtons = new bool[5];
    public static int X { get; private set; }
    public static int Y { get; private set; }

    public static bool IsButtonDown(int button) => PressedButtons[button];

    public static void Update(MouseEvent e)
    {
        switch (e)
        {
            case ButtonEvent mbe:
                PressedButtons[mbe.Button] = mbe.State == ButtonState.Pressed;
                break;
            case MotionEvent mme:
                X = mme.X;
                Y = mme.Y;
                break;
        }
    }
}