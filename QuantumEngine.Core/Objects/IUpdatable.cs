using QuantumCore.Time;

namespace QuantumCore.Objects;

public interface IUpdatable : IHasActorStatus
{
    /// <summary>
    /// Подгатовка перед обновлениями
    /// </summary>
    void PreUpdate(ClockMeta clockMeta);
    
    /// <summary>
    /// Обновления
    /// </summary>
    void Update(ClockMeta clockMeta);
    
    /// <summary>
    /// Завершение обновления
    /// </summary>
    void PostUpdate(ClockMeta clockMeta);
}