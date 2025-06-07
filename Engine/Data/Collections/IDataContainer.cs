namespace Engine.Data.Collections;

[Obsolete("The class is mainly used in the engine, you should not use it in the game.")]
public interface IDataContainer
{
    Identifier Id { get; }

    public ConcurrentIdentifierMap<object> Data { get; init; }

    public object? Get(object key);

    #region Encapsulation

    public bool Contains(Identifier key);
    public bool IsEmpty();
    public int Count { get; }

    public ICollection<Identifier> Keys { get; }

    #endregion
}