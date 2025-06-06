namespace Engine.Data.Collections;

using RegistryManagers;
using Meta;

[Obsolete("The class is mainly used in the engine, you should not use it in the game.")]
public abstract class DataContainer<T> : MetaObject<MetaData>, IDataContainer
{
    public ConcurrentIdentifierMap<object> Data { get; init; }

    #region Constructors

    protected DataContainer(MetaData metaData)
        : this(metaData, new List<KeyValuePair<Identifier, T>>())
    {
    }

    protected DataContainer(MetaData metaData, IEnumerable<KeyValuePair<Identifier, T>> data)
        : base(metaData)
    {
        Data = (ConcurrentIdentifierMap<object>)data;
        Registries.DataContainerRegistry.Register(
            this
        );
    }

    #endregion

    public static IDataContainer? GetContainer(object identifier) => Registries.DataContainerRegistry.Get(identifier);

    #region Item support

    public T? this[Identifier key]
    {
        get => (T)Get(key)!;
        set => Set(key, value);
    }

    public object? Get(Identifier key) => Data.GetValueOrDefault(key);

    public virtual void Set(Identifier key, T? value)
    {
    }

    #endregion

    #region Encapsulation

    public bool Contains(Identifier key) => Data.ContainsKey(key);
    public bool IsEmpty() => Data.IsEmpty;
    public int Count => Data.Count;

    public ICollection<Identifier> Keys => Data.Keys;
    public ICollection<T> Values => (ICollection<T>)Data.Values;

    #endregion
}