namespace Engine.Objects;

public interface IPreparableInstance
{
    /// <summary>
    /// Подгатовка, осуществляющаяся до создания инстанса
    /// </summary>
    /// <remarks>
    /// В основном не вызывается автоматически
    /// </remarks>
    static abstract void PrepareInstance();
}

public interface IPreparableInstance<out T> where T : notnull
{
    /// <summary>
    /// Подгатовка, осуществляющаяся до создания инстанса
    /// </summary>
    /// <remarks>
    /// В основном не вызывается автоматически
    /// </remarks>
    static abstract T PrepareInstance();
}