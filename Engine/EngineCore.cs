using System.Reflection;

namespace Engine;

using Configuration;
using Extensions.Tracer;
using Failures;

/// <summary>
/// Стандартный обработчик ошибок
/// </summary>
internal sealed class ConsoleFailureHandler : IFailureHandler
{
    public void OnFailure(FailureException failure)
    {
        Console.WriteLine($"!!! FAILURE !!! [{failure.Timestamp:HH:mm:ss.fff}]");
        Console.WriteLine($"ID: {failure.CatchId}");
        Console.WriteLine($"Level: {failure.Level}");
        Console.WriteLine($"Message: {failure.Message}");

        if (failure.InnerException != null)
        {
            Console.WriteLine($"Inner exception: {failure.InnerException.GetType().Name}");
            Console.WriteLine(failure.InnerException.Message);
        }

        Console.WriteLine(new string('=', 50));
    }
}

/// <summary>
/// Ядро движка: <br/>
/// * Инициализация <br/>
/// * Запуск AppLib <br/>
/// * Хранение базовых данных <br/>
/// </summary>
public static class EngineCore
{
    public static string RootDirectory = "";
    public static Assembly AppLibAssembly = null!;
    public static IFailureHandler? DefaultFailureHandler;
    // new(identifier: "Main EngineCore Catch")

    static EngineCore()
    {
        DefaultFailureHandler = new ConsoleFailureHandler();
        // Initialize();
        
#if !DEBUG
        BaseConfig.DebugMode = false;
#endif
    }

    [Obsolete("Use only one times after game initialization")]
    public static void Initialize()
    {
        Console.WriteLine("| INFO    | Engine Initialization Started");

        Console.WriteLine($"In Headless mode: {BaseConfig.Headless}");
        Console.WriteLine($"App name: {BaseConfig.AppName}");
        Console.WriteLine($"App path: {RootDirectory}");
        Console.WriteLine($"Asset path: {BaseConfig.AssetPath}");

        Console.WriteLine("| SUCCESS | Engine initialized");

        if (BaseConfig.DebugMode)
        {
            EnableDebugFeatures();
        }

        QuantumTracer.HandleAssembly();
    }

    private static void EnableDebugFeatures()
    {
        Console.WriteLine("| DEBUG   | Enable Debug Features");
    }
}