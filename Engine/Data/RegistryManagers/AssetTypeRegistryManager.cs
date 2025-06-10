using System.Collections.Concurrent;

namespace Engine.Data.RegistryManagers;

using Asset;
using Logging;

internal sealed class AssetTypeRegistryManager : IRegistryManager<AssetType>
{
    private static ConcurrentDictionary<string, AssetType> AssetTypes { get; } = new();
    private static readonly Lazy<AssetTypeRegistryManager> Registry = new(() => new AssetTypeRegistryManager());

    public static IRegistryManager<AssetType> Instance() => Registry.Value;

    public void Register(AssetType obj)
    {
        Logger.Info($"Register AssetType<{obj.AssetLoader.GetType().Name}> - {obj.Name}");
        AssetTypes.TryAdd(obj.Name, obj);
    }

    public AssetType? Get(object id)
    {
        if (id is string str)
            return AssetTypes.GetValueOrDefault(str);
        return null;
    }
}