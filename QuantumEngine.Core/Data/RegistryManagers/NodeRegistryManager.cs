using System.Collections.Concurrent;

namespace Engine.Data.RegistryManagers;

public class NodeRegistryManager : IRegistryManager<object>
{
    private static ConcurrentDictionary<Identifier, object> Nodes { get; } = new([]);
    private static readonly Lazy<NodeRegistryManager> Registry = new(() => new NodeRegistryManager());

    public static NodeRegistryManager Instance() => Registry.Value;

    public void Register(object id, object node)
    {
        var identifier = Identifier.FromUncertain(id);
        Nodes.TryAdd(identifier, node);
    }

    [Obsolete]
    public void Register(object obj)
    {
        throw new NotImplementedException();
    }

    public object? Get(object id)
    {
        var identifier = Identifier.FromUncertain(id);
        return Nodes.GetValueOrDefault(identifier);
    }

    public object? Pop(object id)
    {
        var identifier = Identifier.FromUncertain(id);
        Nodes.Remove(identifier, out var node);
        return node;
    }

    public int Size => Nodes.Count;
    public ICollection<object> Values => Nodes.Values.ToHashSet();
}