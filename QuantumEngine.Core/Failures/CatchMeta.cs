namespace QuantumCore.Failures;

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
    /// <summary>
    /// Handler if exist
    /// </summary>
    public IFailureHandler? Handler { get; } = handler;

    /// <summary>
    ///  level for Failures
    /// </summary>
    public FailureLevel FailureLevel { get; } = failureLevel;

    /// <summary>
    /// Roster of caught Failures
    /// </summary>
    public FailuresRoster Failures { get; } = new(
        new MetaData(
            $"failuresRoster-{identifier}"
        )
    );

    /// <summary>
    /// Показатель работы Catch, равен true при запуске Handle
    /// </summary>
    public bool IsRunning { get; internal set; }

    /// <summary>
    /// Показатель успешности выполнения, равен false при отлове хотя-бы одной ошибки
    /// </summary>
    public bool IsSuccess => Failures.IsEmpty();

    /// <summary>
    /// Показатель обработанности выводов, равен true в случае если не было отловлено ни одной ошибки,
    /// или отловленная ошибка была успешно обработана
    /// </summary>
    public bool IsHandled { get; internal set; }
}