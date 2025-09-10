namespace Engine.Threading;

using Data.Meta;
using Failures;

/// <summary>
/// Метаданные потока
/// </summary>
/// <param name="name">Имя потока</param>
/// <param name="failureLevel">Уровень ошибок в нём</param>
/// <param name="isBackground">Возможность работы потока, после остановки главного</param>
/// <param name="lifetimeSeconds">Время жизни потока, после запуска</param>
public class QThreadMeta(
    string? name,
    FailureLevel? failureLevel,
    bool isBackground,
    float? lifetimeSeconds
) : TimedMetaData(name, lifetimeSeconds ?? EngineCore.Core.Configuration.Get<int>("engine.threading.lifetime"))
{
    public FailureLevel FailureLevel { get; } = failureLevel ?? EngineCore.Core.Configuration.Get<FailureLevel>("engine.threading.failure.level");
    public bool IsBackground { get; } = isBackground;
}