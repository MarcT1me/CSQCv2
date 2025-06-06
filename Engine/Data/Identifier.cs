namespace Engine.Data;

using RegistryManagers;

public sealed class Identifier
{
    public string? Name { get; }
    public Guid Uuid { get; } = Guid.NewGuid();
    
    public static implicit operator Identifier(string? name) => new(name);

    public Identifier(string? name = null)
    {
        Name = name;
        Registries.IdentifierRegistry.Register(this);
    }

    public static Identifier FromUncertain(object? value) => value switch
    {
        Identifier id => id,
        string s => new Identifier(s),
        _ => new Identifier()
    };

    public static Identifier? GiveFromUncertain(object value) => Registries.IdentifierRegistry.Get(value);

    public override string ToString() => Name ?? Uuid.ToString();
}