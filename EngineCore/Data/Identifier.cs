namespace Engine.Data;

using RegistryManagers;

/// <summary>
/// Уникальный идентификатор объекта движка в системе реестров, может инициализироваться из object?
/// </summary>
public sealed class Identifier : IDisposable
{
    public string? Name { get; }
    public Guid Uuid { get; } = Guid.NewGuid();

    public static implicit operator Identifier(string? name) => new(name);

    public Identifier(string? name = null)
    {
        Name = name;
        CoreRegistries.IdentifierRegistry.Register(this);
    }

    /// <summary>
    /// Создаёт идентификатор из неопределённого типа. <br/>
    /// * Identifier - возвращает без изменений, <br/>
    /// * string - превращает в такой-же именованный Identifier, <br/>
    /// * всё остальное, включая null - превращает в неименованный Identifier
    /// </summary>
    /// <param name="value">Объект, который стоит превратить в Identifier</param>
    /// <returns>Уникальный идентификатор, созданный из неопределённого типа</returns>
    public static Identifier FromUncertain(object? value) => value switch
    {
        Identifier id => id,
        string s => new Identifier(s),
        _ => new Identifier(value?.ToString())
    };

    /// <summary>
    /// Выдаёт хранящийся в реестре идентификатор
    /// </summary>
    /// <param name="value">Объект, предположительно лежащий в реестре, как Identifier</param>
    /// <returns>Первый совпавший с value идентификатор</returns>
    public static Identifier? GiveFromUncertain(object value) =>
        CoreRegistries.IdentifierRegistry.Get(value);

    public override string ToString() => Name ?? Uuid.ToString();
    public string GetNameAnyway() => Name ?? "Unnamed";
    
    public void Dispose()
    {
        CoreRegistries.IdentifierRegistry.Pop(this);
        GC.SuppressFinalize(this);
    }
    
    ~Identifier() => Dispose();
}