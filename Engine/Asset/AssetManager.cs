namespace Engine.Asset;

using Data.Meta;
using Data.Arrays;
using Data.RegistryManagers;

public sealed class AssetManager(DependencyResolver resolver)
{
    public Roster<AssetData> Storage { get; } = new(new MetaData("assetStorage"));

    public static void RegisterAssetType(AssetType type)
    {
        Registries.AssetTypeRegistry.Register(type);
    }

    public AssetData Load(AssetFile assetFile, IEnumerable<AssetData>? alreadyLoadedDependencies = null)
    {
        var assetType = Registries.AssetTypeRegistry.Get(assetFile.TypeName);
        if (assetType == null)
            throw new InvalidAssetTypeError(assetFile);

        try
        {
            // get or create new asset branch
            var branch = Storage.GetBranch(
                assetType.Name
            ) ?? Storage.NewBranch(
                new MetaData(
                    "assetStorage-branch-" + assetType.Name
                )
            );

            // resolve dependencies
            var resolvedDependencies = resolver.Resolve(assetFile);

            // load content from file 
            var loadedContent = assetType.AssetLoader.LoadFile(assetFile);

            // create asset data instance
            var finalDependencies = resolvedDependencies.Concat(alreadyLoadedDependencies ?? []);
            var assetData = assetType.AssetLoader.CreateAsset(assetFile, finalDependencies, loadedContent);

            // save in asset branch
            branch[assetData.Identifier] = assetData;

            return assetData;
        }
        catch (Exception e)
        {
            throw new AssetError($"Failed to load asset from {assetFile.Path}", e);
        }
    }
}