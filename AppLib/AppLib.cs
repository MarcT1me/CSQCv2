namespace AppLib;

using Engine.Logging;

public class AppLib
{
    public static void Activate()
    {
        Logger.Debug("Activating project from a Luncher");
    }

    public static void Start()
    {
        Logger.Debug("Start App from a Luncher");
        Game.StartGame();
    }
}