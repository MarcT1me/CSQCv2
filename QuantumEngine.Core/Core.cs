using System.Reflection;
using QuantumCore.Asset.Default;

namespace QuantumCore;

using Extensions.Tracer;
using Failures;
using Logging;

/// <summary>
/// Стандартный обработчик ошибок
/// </summary>
internal class DefaultFailureHandler : IFailureHandler
{
    public void OnFailure(FailureException failure)
    {
        if (failure.Level != FailureLevel.Second)
        {
            var message = "Default QuantumEngineCore failure handler got a failure\n" +
                          $"Catch: '{failure.CatchId}'\n" +
                          $"Level: '{failure.Level}'\n" +
                          $"Message: {failure.Message}";

            if (failure.InnerException != null)
            {
                message += $"\nInner exception: {failure.InnerException.GetType().Name}";
                message += $"\n{failure.InnerException.Message}";
            }

            Logger.Warning(message);
        }

        failure.RecastIfSecond();
    }
}

/// <summary>
/// Ядро движка: <br/>
/// * Инициализация <br/>
/// * Запуск AppLib <br/>
/// * Хранение базовых данных <br/>
/// </summary>
public class Core
{
    public static string RootDirectory = "";
    public static IFailureHandler DefaultFailureHandler = new DefaultFailureHandler();

    public static Configuration CoreConfiguration = new("QuantumEngine.Core:engine.core.configuration.properties");

    public static bool IsDebug { get; set; } = CoreConfiguration.Get<bool>("app.debug.enabled");

    [Obsolete("ENGINE ONLY USAGE!")]
    public static void Initialize(Assembly? appLibAssembly)
    {
        Logger.InitDefault();

#if !DEBUG
            IsDebug = false;
#endif

        Logger.Info(
            "QuantumEngineCore Initialization Started\n" +
            "Configuration:"
        );
        foreach (var config in CoreConfiguration.IterConfigs())
        {
            Logger.SimpleLog($"\"{config.Key}\": {config.Value} ({config.Value.GetType()})");
        }

        Logger.SimpleLog(
            "\nPaths:\n" +
            $"App path: {RootDirectory}\n" +
            $"Asset path: {RootDirectory + CoreConfiguration.Get<string>("app.asset.dir")}"
        );

        Logger.Separator();

        QuantumTracer.HandleAssembly(
            [
                Assembly.GetExecutingAssembly(), // Engine Core module
                Assembly.GetCallingAssembly(), // Headless or Quantum Engine module
                appLibAssembly, // AppLib module
                Assembly.GetEntryAssembly() // TestApp
            ]
        );

        Logger.Separator();

        Logger.Success("Core Engine module initialized");
        Logger.Separator();
    }
}