using System.Collections.Concurrent;
using System.Reflection;

namespace Engine.Data.RegistryManagers;

internal sealed class ScanMethodRegistryManager : IRegistryManager<MethodInfo>
{
    private static ConcurrentDictionary<string, MethodInfo> ScanMethods { get; } = new();
    private static readonly Lazy<ScanMethodRegistryManager> Registry = new(() => new ScanMethodRegistryManager());

    public static IRegistryManager<MethodInfo> Instance() => Registry.Value;

    public void Register(MethodInfo obj)
    {
        var key = $"{obj.DeclaringType?.FullName}.{obj.Name}";
        ScanMethods.TryAdd(key, obj);
    }

    public MethodInfo? Get(object id)
    {
        if (id is not string name) return null;

        foreach (var (key, value) in ScanMethods)
            if (key.Contains(name))
                return value;

        return null;
    }
}