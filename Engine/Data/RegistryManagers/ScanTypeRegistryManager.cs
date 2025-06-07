using System.Collections.Concurrent;

namespace Engine.Data.RegistryManagers;

internal sealed class ScanTypeRegistryManager : IRegistryManager<Type>
{
    private static ConcurrentDictionary<string, Type> ScanTypes { get; } = new();
    private static readonly Lazy<ScanTypeRegistryManager> Registry = new(() => new ScanTypeRegistryManager());

    public static IRegistryManager<Type> Instance() => Registry.Value;

    public void Register(Type obj)
    {
        ScanTypes[$"{obj}/{obj.Name}"] = obj;
    }

    public Type? Get(object id)
    {
        if (id is not string name) return null;

        foreach (var (key, value) in ScanTypes)
            if (key.Contains(name))
                return value;

        return null;
    }
}