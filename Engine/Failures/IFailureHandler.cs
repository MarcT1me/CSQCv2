namespace Engine.Failures;

/// <summary>
/// Интерфейс реализации обработчика ошибок
/// </summary>
public interface IFailureHandler
{
    /// <summary>
    /// Обработка ошибки
    /// </summary>
    /// <param name="failure">Подготовленный в Catch или в ручную FailureException</param>
    void OnFailure(FailureException failure);
}