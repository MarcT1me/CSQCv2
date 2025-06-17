namespace Engine.Objects;


public interface IExitHandler
{
    /// <summary>
    /// Событие выхода из приложения
    /// </summary>
    /// <remarks>Только для использования с event</remarks>
    void OnExitHandling();
}