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
        var message = "Default EngineCore failure handler got a failure\n" +
                      $"Catch: '{failure.CatchId}'\n" +
                      $"Level: '{failure.Level}'\n" +
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
public abstract class EngineCore
{
    public static string RootDirectory = "";
    public static Assembly? AppLibAssembly;
    public static IFailureHandler DefaultFailureHandler = new ConsoleFailureHandler();

    protected EngineCore(Assembly? appLibAssembly)
    {
        AppLibAssembly = appLibAssembly;

        Logger.InitLogger();

        Logger.Separator();

#if !DEBUG
            BaseConfig.DebugMode = false;
#endif
    }

    [Obsolete("Obsolete")]
    protected void InitializeCore()
    {
        Logger.Info(
            "EngineCore Initialization Started\n" +
            $"Headless: {BaseConfig.Headless}\n" +
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

        if (BaseConfig.DebugMode) EnableDebugFeatures();

        Logger.Separator();
        
        InitializeModeSpecific();

        Logger.Separator();

        Logger.Success("EngineCore initialized");

        Logger.Separator();
    }

    protected abstract void InitializeModeSpecific();

    protected abstract void EnableDebugFeatures();
}