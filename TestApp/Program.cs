using QuantumLauncher;

namespace TestApp;

public class Program
{
    public static void Main(string[] args)
    {
        // load engine and link to loaded App
        var (app, eng) = QLauncher.InitProject(AppDomain.CurrentDomain);
        eng.Activate(); // activate Engine

        // activate and start app
        app.Activate();
        app.Start();
    }
}