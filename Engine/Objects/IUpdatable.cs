namespace Engine.Objects;

public interface IUpdatable : IHasActorStatus
{
    public bool IsDynamic() => ObjectStatus.HasFlag(ObjectStatusFlags.Dynamic);
    
    /// <summary>
    /// Подгатовка перед обновлениями
    /// </summary>
    void PreUpdate();
    
    /// <summary>
    /// Обновления
    /// </summary>
    void Update();
    
    /// <summary>
    /// Завершение обновления
    /// </summary>
    void PostUpdate();
}