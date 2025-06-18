namespace Engine.Objects;


public interface IExitHandler : IDisposable
{
    /// <summary>
    /// Событие выхода из приложения
    /// </summary>
    /// <remarks>Только для использования с event</remarks>
    void OnExitHandling();
}