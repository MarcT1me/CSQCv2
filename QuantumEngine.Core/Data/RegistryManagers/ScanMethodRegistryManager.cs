using System.Collections.Concurrent;

namespace QuantumCore.Data.RegistryManagers;

using Decorators;

internal sealed class ScanMethodRegistryManager : IRegistryManager<QuantumMethodInfo>
{
    private static ConcurrentDictionary<string, QuantumMethodInfo> ScanMethods { get; } = new();
    private static readonly Lazy<ScanMethodRegistryManager> Registry = new(() => new ScanMethodRegistryManager());

    public static IRegistryManager<QuantumMethodInfo> Instance() => Registry.Value;

    public void Register(QuantumMethodInfo qMethodgod)
    {
        var key = $"{qMethodgod.Method.DeclaringType?.FullName}.{qMethodgod.Method.Name}";
        ScanMethods.TryAdd(key, qMethodgod);
    }

    public QuantumMethodInfo? Get(object id)
    {
        if (id is not string name) return null;

        foreach (var (key, value) in ScanMethods)
            if (key.Contains(name))
                return value;

        return null;
    }

    public QuantumMethodInfo? Pop(object id)
    {
        if (id is not string k) return null;
        ScanMethods.Remove(k, out var type);
        return type;
    }

    public int Size => ScanMethods.Count;
    public ICollection<QuantumMethodInfo> Values => ScanMethods.Values;
}