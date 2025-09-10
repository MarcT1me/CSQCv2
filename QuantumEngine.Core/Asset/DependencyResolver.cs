using QuantumCore.Data.Collections;

namespace QuantumCore.Asset;

using Logging;

public static class DependencyResolver
{
    private static readonly ConcurrentSet<string> LoadedSet = new();

    public static LinkedList<AssetData> Resolve(
        AssetFile assetFile
    )
    {
        LinkedList<AssetData> dependencies = new();

        if (assetFile.Dependencies.Count == 0)
            return dependencies;
        
        Logger.Info($"Resolving asset dependencies for {assetFile.Identifier}");

        var cacheKey = assetFile.Path;

        if (!LoadedSet.Contains(cacheKey))
            throw new CyclicDependencyError(assetFile);

        LoadedSet.Add(cacheKey);

        AssetFile? currentDependency = null;
        try
        {
            var assetDatas = assetFile.Dependencies
                .Select(dep => AssetManager.Load(dep))
                .ToList();

            foreach (var data in assetDatas) 
                dependencies.AddLast(data);

            return dependencies;
        }
        catch (Exception e)
        {
            throw new AssetError(
                $"Failed to load  dependency {currentDependency!.Identifier} for asset {assetFile.Identifier}", e
            );
        }
        finally
        {
            LoadedSet.Remove(cacheKey);
        }
    }
}