using System.Collections.Concurrent;

namespace Engine.Data.RegistryManagers;

internal sealed class IdentifierRegistryManager : IRegistryManager<Identifier>
{
    // Заменяем ConcurrentBag на ConcurrentDictionary для эффективного удаления
    private static readonly ConcurrentDictionary<Guid, Identifier> Identifiers = new();
    private static readonly Lazy<IdentifierRegistryManager> Registry = new(() => new IdentifierRegistryManager());

    public static IRegistryManager<Identifier> Instance() => Registry.Value;

    public void Register(Identifier identifier)
    {
        Identifiers.TryAdd(identifier.Uuid, identifier);
    }

    public Identifier? Get(object id) => id switch
    {
        Identifier identifier => Identifiers.GetValueOrDefault(identifier.Uuid),
        string s => Identifiers.Values.FirstOrDefault(i => i.Name != null && i.Name.Equals(s)),
        Guid uuid => Identifiers.GetValueOrDefault(uuid),
        _ => Identifiers.Values.FirstOrDefault(i => i.Name != null && i.Name.Equals(id.ToString()))
    };

    public Identifier? Pop(object id)
    {
        Identifier? target = Get(id);
        if (target == null) 
            return null;

        Identifiers.TryRemove(target.Uuid, out _);
        return target;
    }

    public int Size => Identifiers.Count;
    public ICollection<Identifier> Values => Identifiers.Values;
}