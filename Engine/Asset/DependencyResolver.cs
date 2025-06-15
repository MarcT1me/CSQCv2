using Engine.Data.Collections;

namespace Engine.Asset;

using Logging;

public static class DependencyResolver
{
    private static readonly ConcurrentHashSet<string> LoadedSet = new();

    public static async Task<LinkedList<AssetData>> ResolveAsync(
        AssetFile assetFile,
        CancellationToken ct = default
    )
    {
        Logger.Info($"Resolving asset dependencies for {assetFile.Identifier}");

        LinkedList<AssetData> dependencies = new();

        if (assetFile.Dependencies.Count == 0)
            return dependencies;

        var cacheKey = assetFile.Path;

        if (!LoadedSet.Contains(cacheKey))
            throw new CyclicDependencyError(assetFile);

        LoadedSet.Add(cacheKey);

        AssetFile? currentDependency = null;
        try
        {
            var loadTasks = assetFile.Dependencies
                .Select(dep => AssetManager.LoadAsync(dep, ct: ct))
                .ToList();
            
            await Task.WhenAll(loadTasks);
            foreach (var task in loadTasks) dependencies.AddLast(task.Result);

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