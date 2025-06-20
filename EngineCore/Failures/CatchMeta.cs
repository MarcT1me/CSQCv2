namespace Engine.Failures;

using Data.Meta;
using Data;

/// <summary>
/// Метаданные Catch
/// </summary>
/// <param name="identifier">Идентификатор Catch</param>
/// <param name="failureLevel">Уровень критичности ошибки</param>
/// <param name="handler">Обработчик ошибки</param>
public sealed class CatchMeta(
    object? identifier,
    FailureLevel failureLevel,
    IFailureHandler? handler
) : TimedMetaData(Identifier.FromUncertain(identifier))
{
    public IFailureHandler? Handler { get; } = handler;
    public FailureLevel FailureLevel { get; } = failureLevel;

    public FailuresRoster Failures { get; } = new(
        new MetaData(
            $"failuresRoster-{identifier}"
        )
    );
}