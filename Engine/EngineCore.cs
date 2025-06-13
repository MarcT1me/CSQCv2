using System.Reflection;

namespace Engine;

using Configuration;
using Extensions.Tracer;
using Failures;
using Logging;

/// <summary>
/// Стандартный обработчик ошибок
/// </summary>
internal sealed class ConsoleFailureHandler : IFailureHandler
{
    public void OnFailure(FailureException failure)
    {
        var message = "Default Engine failure handler got a failure\n" +
                      $"ID: {failure.CatchId}\n" +
                      $"Level: {failure.Level}\n" +
                      $"Message: {failure.Message}";

        if (failure.InnerException != null)
        {
            message += $"\nInner exception: {failure.InnerException.GetType().Name}";
            message += $"\n{failure.InnerException.Message}";
        }

        Logger.Warning(message);

        failure.Handle();
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
    public static Assembly? AppLibAssembly;
    public static IFailureHandler? DefaultFailureHandler;

    public static void Initialize(Assembly? appLibAssembly)
    {
        Console.WriteLine("Initialize EngineCore");
        
        using (new Catch("Main EngineCore Catch"))
        {
            AppLibAssembly = appLibAssembly;
            DefaultFailureHandler = new ConsoleFailureHandler();

#if !DEBUG
        BaseConfig.DebugMode = false;
#endif

#pragma warning disable CS0618 // Type or member is obsolete
            InitializeCore();
#pragma warning restore CS0618 // Type or member is obsolete
        }
    }

    [Obsolete("Use only one times after game initialization")]
    private static void InitializeCore()
    {
        Logger.InitLogger();

        Logger.Info("Engine Initialization Started");

        Logger.Info($"In Headless mode: {BaseConfig.Headless}\n" +
                    $"App name: {BaseConfig.AppName}\n" +
                    $"App path: {RootDirectory}\n" +
                    $"Asset path: {BaseConfig.AssetPath}"
        );
        Logger.Separator();
        
        QuantumTracer.HandleAssembly(
            [
                Assembly.GetExecutingAssembly(),
                AppLibAssembly
            ]
        );
        Logger.Separator();

        if (BaseConfig.DebugMode)
        {
            EnableDebugFeatures();
        }

        Logger.Success("Engine initialized");
        Logger.Separator();
    }

    private static void EnableDebugFeatures()
    {
        Logger.Debug("Enable Debug Features");
    }
}