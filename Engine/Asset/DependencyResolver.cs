using Engine.Data.Collections;

namespace Engine.Asset;

using Logging;

public class DependencyResolver(AssetManager manager)
{
    private readonly ConcurrentHashSet<string> _loadedSet = new();

    public async Task<LinkedList<AssetData>> ResolveAsync(
        AssetFile assetFile,
        CancellationToken ct = default
    )
    {
        Logger.Info($"Resolving asset dependencies for {assetFile.Identifier}");

        LinkedList<AssetData> dependencies = new();

        if (assetFile.Dependencies.Count == 0)
            return dependencies;

        var cacheKey = assetFile.Path;

        if (!_loadedSet.Contains(cacheKey))
            throw new CyclicDependencyError(assetFile);

        _loadedSet.Add(cacheKey);

        AssetFile? currentDependency = null;
        try
        {
            var loadTasks = assetFile.Dependencies
                .Select(dep => manager.LoadAsync(dep, ct: ct))
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
            _loadedSet.Remove(cacheKey);
        }
    }
}