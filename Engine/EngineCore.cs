using System.Reflection;

namespace Engine;

using Configuration;
using Extensions.Tracer;
using Failures;
using Logging;
using Graphic.Window;
using Extensions;

/// <summary>
/// Стандартный обработчик ошибок
/// </summary>
internal sealed class ConsoleFailureHandler : IFailureHandler
{
    public void OnFailure(FailureException failure)
    {
        var message = "Default Engine failure handler got a failure\n" +
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
public static class EngineCore
{
    public static string RootDirectory = "";
    public static Assembly? AppLibAssembly;
    public static IFailureHandler DefaultFailureHandler = new ConsoleFailureHandler();

    public static void Initialize(Assembly? appLibAssembly)
    {
        With.Handle(new Catch("Main EngineCore Catch"), _ =>
        {
            AppLibAssembly = appLibAssembly;

#if !DEBUG
        BaseConfig.DebugMode = false;
#endif

#pragma warning disable CS0618 // Type or member is obsolete
            InitializeCore();
#pragma warning restore CS0618 // Type or member is obsolete
        });
    }

    [Obsolete("Use only one times after game initialization")]
    private static void InitializeCore()
    {
        Logger.InitLogger();

        Logger.Separator();

        Logger.Info(
            "Engine Initialization Started\n" +
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

        Logger.Info("Initialize SDL");
        Window.InitialiseSdl();

        Logger.Separator();

        if (BaseConfig.DebugMode) EnableDebugFeatures();

        Logger.Separator();
        Logger.Success("Engine initialized");
    }

    private static void EnableDebugFeatures()
    {
        Logger.Debug("Enable Debug Features");
    }

    public static void Uninitialize()
    {
        Logger.Info("Uninitialize EngineCore");

        Logger.Info("Initialize SDL");
        Window.UninitialiseSdl();

        Logger.Success("Engine uninitialized");
    }
}