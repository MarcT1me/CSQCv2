using QuantumLauncher;

namespace TestApp;

public class Program
{
    public static void Main(string[] args)
    {
        // load engine and link to loaded App
        var (app, eng) = QLauncher.InitProject(AppDomain.CurrentDomain);

        // Activate EngineCore
        eng.Activate();

        // activate and start app
        app.Start();

        // Deactivate EngineCore
        eng.Deactivate();
    }
}