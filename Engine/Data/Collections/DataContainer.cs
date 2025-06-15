namespace Engine.Data.Collections;

using RegistryManagers;
using Meta;

[Obsolete("The class is mainly used in the engine, you should not use it in the game.")]
public abstract class DataContainer<T> : MetaObject<MetaData>, IDataContainer
{
    public ConcurrentIdentifierMap<object> Data { get; init; }

    #region Constructors

    protected DataContainer(MetaData metaData)
        : this(metaData, new Dictionary<Identifier, T>())
    {
    }

    protected DataContainer(MetaData metaData, Dictionary<Identifier, T> data)
        : base(metaData)
    {
        Data = new ConcurrentIdentifierMap<object>(data as Dictionary<Identifier, object>);
        Registries.DataContainerRegistry.Register(
            this
        );
    }

    #endregion

    public static IDataContainer? GetContainer(object identifier) => Registries.DataContainerRegistry.Get(identifier);

    #region Item support

    public T? this[object key]
    {
        get => (T)Get(key)!;
        set => Set(key, value);
    }

    /// <summary>
    /// Выдаёт данные из контейнера
    /// </summary>
    /// <param name="key">Уникальный идентификатор данных в контейнере</param>
    /// <returns>Данные в чистом виде, если есть</returns>
    public object? Get(object key) =>
        Identifier.GiveFromUncertain(key) is { } identifier ? Data.GetValueOrDefault(identifier) : null;

    /// <summary>
    /// Сохраняет данные в контейнер
    /// </summary>
    /// <param name="key">Объект, ассоциируемый со значением</param>
    /// <param name="value">Значение на сохранение</param>
    public virtual void Set(object key, T? value)
    {
    }    
    
    /// <summary>
    /// Вынимает данные из контейнера
    /// </summary>
    /// <param name="key">Объект, ассоциируемый со значением</param>
    /// <returns>Данные, лежащие по ключу или пустоту, если их нет</returns>
    public virtual object? Pop(object key)
    {
        return null;
    }

    #endregion

    #region Encapsulation

    public bool Contains(Identifier key) => Data.ContainsKey(key);
    public bool IsEmpty() => Data.IsEmpty;
    public int Count => Data.Count;

    public ICollection<Identifier> Keys => Data.Keys;
    public ICollection<object> Values => Data.Values;
    
    public void Clear() => Data.Clear();

    #endregion
}