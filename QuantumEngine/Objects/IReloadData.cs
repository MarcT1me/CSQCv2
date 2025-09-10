namespace QuantumCore.Objects;

using Data.Meta;

public interface IReloadData : IDisposable
{
    /// <summary>
    /// Событие перезагрузки данных
    /// </summary>
    /// <remarks>Как для использования с event так и для вызова</remarks>
    void OnReloadData(MetaData data);
}