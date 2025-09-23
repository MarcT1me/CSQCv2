namespace QuantumCore.Data.Collections;

using RegistryManagers;
using Meta;

[Obsolete("The class is mainly used in the engine, you should not use it in the game.")]
public abstract class DataContainer<T> : MetaObject<MetaData>, IDataContainer, IDisposable
{
    public ConcurrentIdentifierMap<object> Data { get; init; }
    protected readonly object? Lock;

    #region Constructors

    protected DataContainer(MetaData metaData, object? lockObject = null)
        : this(metaData, new Dictionary<Identifier, T>(), lockObject)
    {
    }

    protected DataContainer(MetaData metaData, Dictionary<Identifier, T> data, object? lockObject = null)
        : base(metaData)
    {
        Data = new ConcurrentIdentifierMap<object>(data as Dictionary<Identifier, object>);
        CoreRegistries.DataContainerRegistry.Register(
            this
        );
        Lock = lockObject;
    }

    #endregion

    public static IDataContainer? GetContainer(object identifier) =>
        CoreRegistries.DataContainerRegistry.Get(identifier);

    #region Item support

    public T this[object key]
    {
        get => (T)Get(key)! ?? throw new KeyNotFoundException();
        set => Set(key, value);
    }

    /// <summary>
    /// Выдаёт данные из контейнера
    /// </summary>
    /// <param name="key">Уникальный идентификатор данных в контейнере</param>
    /// <returns>Данные в чистом виде, если есть</returns>
    public object? Get(object key)
    {
        if (Identifier.GiveFromUncertain(key) is not { } identifier)
        {
            return null;
        }

        if (Lock is null)
        {
            return Data.GetValueOrDefault(identifier);
        }

        lock (Lock)
        {
            return Data.GetValueOrDefault(identifier);
        }
    }

    /// <summary>
    /// Сохраняет данные в контейнер
    /// </summary>
    /// <param name="key">Объект, ассоциируемый со значением</param>
    /// <param name="value">Значение на сохранение</param>
    public virtual void Set(object key, T? value)
    {
        throw new NotImplementedException("The \'Set\' method is not implemented.");
    }

    /// <summary>
    /// Вынимает данные из контейнера
    /// </summary>
    /// <param name="key">Объект, ассоциируемый со значением</param>
    /// <returns>Данные, лежащие по ключу или пустоту, если их нет</returns>
    public virtual T? Pop(object key)
    {
        throw new NotImplementedException("The \'Pop\' method is not implemented.");
    }

    #endregion

    #region Encapsulation

    public bool Contains(Identifier key) => Data.ContainsKey(key);
    public bool IsEmpty() => Data.IsEmpty;
    public int Count => Data.Count;

    public ICollection<Identifier> Keys => Data.Keys;
    public ICollection<object> Values => Data.Values;

    public void Clear() => Data.Clear();

    public bool ContainsKey(Identifier key) => Data.ContainsKey(key);

    #endregion

    public virtual void Dispose()
    {
        CoreRegistries.DataContainerRegistry.Pop(Id);
        GC.SuppressFinalize(this);
    }

    ~DataContainer() => Dispose();
}