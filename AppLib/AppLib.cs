namespace AppLib;

using Game;

public static class AppLib
{
    public static void Start()
    {
        TestApp.Mainloop(typeof(TestApp));
    }
}