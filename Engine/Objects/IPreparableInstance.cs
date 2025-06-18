namespace Engine.Objects;

public interface IPreparableInstance
{
    /// <summary>
    /// Подгатовка, осуществляющаяся до создания инстанса
    /// </summary>
    /// <remarks>
    /// В основном не вызывается автоматически
    /// </remarks>
    void PrepareInstance();
}

public interface IPreparableInstance<T> where T : class
{
    /// <summary>
    /// Подгатовка, осуществляющаяся до создания инстанса
    /// </summary>
    /// <remarks>
    /// В основном не вызывается автоматически
    /// </remarks>
    T PrepareInstance();
}