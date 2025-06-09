namespace Engine.Asset;

using Logging;

public sealed class DependencyResolver(AssetManager manager)
{
    private readonly HashSet<string> _loadedSet = [];

    public LinkedList<AssetData> Resolve(AssetFile assetFile)
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
            foreach (var dependency in assetFile.Dependencies)
            {
                currentDependency = dependency;
                dependencies.AddLast(
                    manager.Load(dependency)
                );
            }
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

        return dependencies;
    }
}