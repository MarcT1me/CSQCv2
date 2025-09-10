using System.Collections.Concurrent;

namespace QuantumCore.Data.RegistryManagers;

internal sealed class ScanTypeRegistryManager : IRegistryManager<Type>
{
    private static ConcurrentDictionary<string, Type> ScanTypes { get; } = new();
    private static readonly Lazy<ScanTypeRegistryManager> Registry = new(() => new ScanTypeRegistryManager());

    public static IRegistryManager<Type> Instance() => Registry.Value;

    public void Register(Type obj)
    {
        var key = $"{obj}/{obj.Name}";
        ScanTypes.TryAdd(key, obj);
    }

    public Type? Get(object id)
    {
        if (id is not string name) return null;

        foreach (var (key, value) in ScanTypes)
            if (key.Contains(name))
                return value;

        return null;
    }

    public Type? Pop(object key)
    {
        if (key is not string k) return null;
        ScanTypes.Remove(k, out var type);
        return type;
    }

    public int Size => ScanTypes.Count;
    public ICollection<Type> Values => ScanTypes.Values;
}