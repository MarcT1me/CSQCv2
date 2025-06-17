namespace Engine.Objects;

public interface IPreparable
{
    /// <summary>
    /// Подготовка к выполнению каких-либо методов
    /// </summary>
    /// <remarks>
    /// В основном не вызывается автоматически
    /// </remarks>
    void Prepare();
}