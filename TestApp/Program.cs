using QuantumLauncher;

namespace TestApp;

public class Program
{
    public static void Main(string[] args)
    {
        AppLibModule? app;
        EngineModule? eng;
        
        try
        {
            // load engine and link to loaded App
            (app, eng) = QLauncher.InitProject(AppDomain.CurrentDomain);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"QLauncher.InitProject Failure:\n{ex}");
            return;
        }

        try
        {
            // Activate QuantumEngine.Core
            eng.Activate();
            // activate and start app
            app.Start();
        }
        finally
        {
            // Deactivate QuantumEngine.Core
            eng.Deactivate();
        }
    }
}