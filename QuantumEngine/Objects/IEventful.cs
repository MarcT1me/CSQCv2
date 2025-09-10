namespace QuantumCore.Objects;

using Events.QuantumEvents;

public interface IEventful : IHasActorStatus, IDisposable
{
    /// <summary>
    /// Обработчик событий (как для работы с event, так и для прямых вызовов)
    /// </summary>
    /// <param name="e">Событие для обработки</param>
    void HandleEvent(QuantumEvent e);
}