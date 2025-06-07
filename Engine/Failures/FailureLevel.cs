namespace Engine.Failures;

/// <summary>
/// Уровни критичности ошибок: <br/>
/// * NotCritical - полностью игнорируется, <br/>
/// * First - передаётся на обработчик, <br/>
/// * Second - выбрасывается повторно
/// </summary>
public enum FailureLevel
{
    NotCritical = 0,
    First = 1,
    Second = 2
}