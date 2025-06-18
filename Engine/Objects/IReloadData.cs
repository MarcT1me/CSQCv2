namespace Engine.Objects;

public interface IReloadData
{
    /// <summary>
    /// Событие перезагрузки данных
    /// </summary>
    /// <remarks>Как для использования с event так и для вызова</remarks>
    void OnReloadData();
}