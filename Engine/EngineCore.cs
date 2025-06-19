using System.Reflection;
using Engine.Graphic.OpenGl;
using MirageAPI;

namespace Engine;

using Configuration;
using Extensions.Tracer;
using Failures;
using Logging;
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

    [Obsolete("Obsolete")]
    public static void Initialize(Assembly? appLibAssembly)
    {
        With.Handle(new Catch("Main EngineCore Catch"), _ =>
        {
            AppLibAssembly = appLibAssembly;

            Logger.InitLogger();

            Logger.Separator();

#if !DEBUG
        BaseConfig.DebugMode = false;
#endif

            InitializeCore();

            Logger.Info("Initialize MirageSystem");
            if (!MirageSystem.init(GlData.ApiVersions.X, GlData.ApiVersions.Y))
            {
                Logger.Error("Failed to initialize MirageSystem");
            }

            Logger.Info("Initialize All SDL");
            SDL2.SDL.SDL_Init(SDL2.SDL.SDL_INIT_EVERYTHING);
        });
    }

    [Obsolete("Obsolete")]
    private static void InitializeCore()
    {
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

        Logger.Success("Engine uninitialized");
    }
}