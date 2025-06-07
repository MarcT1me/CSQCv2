using System.Collections.Concurrent;

namespace Engine.Data.RegistryManagers;

internal sealed class IdentifierRegistryManager : IRegistryManager<Identifier>
{
    private static ConcurrentBag<Identifier> Identifiers { get; } = [];
    private static readonly Lazy<IdentifierRegistryManager> Registry = new(() => new IdentifierRegistryManager());

    public static IRegistryManager<Identifier> Instance() => Registry.Value;

    public void Register(Identifier identifier)
    {
        Identifiers.Add(identifier);
    }

    public Identifier? Get(object? id) => id switch
    {
        Identifier identifier => identifier,
        string s => Identifiers.FirstOrDefault(identifier => identifier.Name != null && identifier.Name.Equals(s)),
        Guid uuid => Identifiers.FirstOrDefault(identifier => identifier.Uuid.Equals(uuid)),
        _ => null
    };
}