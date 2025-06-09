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
    public static Assembly AppLibAssembly = null!;
    public static IFailureHandler? DefaultFailureHandler;

    static EngineCore()
    {
        using (new Catch("Main EngineCore Catch"))
        {
#pragma warning disable CS0618 // Type or member is obsolete
            DefaultFailureHandler = new ConsoleFailureHandler();

#if !DEBUG
        BaseConfig.DebugMode = false;
#endif

            Initialize();
#pragma warning restore CS0618 // Type or member is obsolete
        }
    }

    [Obsolete("Use only one times after game initialization")]
    public static void Initialize()
    {
        Logger.Info("Engine Initialization Started");

        Logger.Info(
            $"In Headless mode: {BaseConfig.Headless}\n" +
            $"App name: {BaseConfig.AppName}\n" +
            $"App path: {RootDirectory}\n" +
            $"Asset path: {BaseConfig.AssetPath}"
        );

        Logger.Success("Engine initialized");

        if (BaseConfig.DebugMode)
        {
            EnableDebugFeatures();
        }

        QuantumTracer.HandleAssembly(
            [
                Assembly.GetExecutingAssembly(),
                AppLibAssembly
            ]
        );
    }

    private static void EnableDebugFeatures()
    {
        Logger.Debug("Enable Debug Features");
    }
}