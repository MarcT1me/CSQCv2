namespace Engine.Failures;

/// <summary>
/// Уровни критичности ошибок: <br/>
/// * NotCritical - полностью игнорируется, <br/>
/// * First - передаётся на обработчик, <br/>
/// * Second - выбрасывается повторно
/// </summary>
public enum FailureLevel
{
    /// <summary>
    /// Full ignoring
    /// </summary>
    NotCritical = 0,

    /// <summary>
    /// Handling in Except method of Catch and OnFailure method of IFailureHandler
    /// </summary>
    First = 1,

    /// <summary>
    /// Handling in Except method of Catch and OnFailure method of IFailureHandler and re-cast in handling
    /// </summary>
    Second = 2
}