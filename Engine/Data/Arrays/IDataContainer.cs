namespace Engine.Data.Arrays;

public interface IDataContainer
{
    Identifier Id { get; }
    
    public ConcurrentIdentifierMap<object> Data { get; init; }
    
    public object? Get(Identifier key);

    #region Encapsulation

    public bool Contains(Identifier key);
    public bool IsEmpty();
    public int Count { get; }

    public ICollection<Identifier> Keys { get; }

    #endregion
}