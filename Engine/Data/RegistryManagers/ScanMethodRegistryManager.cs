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
        ScanMethods[obj.Name] = obj;
    }

    public MethodInfo? Get(object id)
    {
        if (id is string str)
            return ScanMethods.GetValueOrDefault(str);
        return null;
    }
}