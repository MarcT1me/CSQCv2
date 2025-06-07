namespace Engine.Asset;

using Data.Meta;
using Data.Collections;
using Data.RegistryManagers;

/// <summary>
/// Менеджер, управляющий ассетами
/// </summary>
/// <param name="resolver">Объект управляющий зависимостями ассетов</param>
public sealed class AssetManager(DependencyResolver resolver)
{
    public Roster<AssetData> Storage { get; } = new(new MetaData("assetStorage"));

    /// <summary>
    /// Регистрация ассета
    /// </summary>
    /// <param name="type">Тип ассета</param>
    /// <exception cref="InvalidAssetTypeError">В случае, если тип ассета уже зарегистрирован</exception>
    public static void RegisterAssetType(AssetType type)
    {
        if (Registries.AssetTypeRegistry.Get(type.Name) != null)
            throw new InvalidAssetTypeError(type.Name);
        Registries.AssetTypeRegistry.Register(type);
    }

    /// <summary>
    /// Метод загрузки ассета
    /// </summary>
    /// <param name="assetFile">Данные о загрузке</param>
    /// <param name="alreadyLoadedDependencies">Уже загруженные ассеты, если такие есть</param>
    /// <returns></returns>
    /// <exception cref="InvalidAssetTypeError">В случае, если тип ассета был указан не верно</exception>
    /// <exception cref="AssetError">В любых других случаях, если ассет не был загружен до конца</exception>
    public AssetData Load(AssetFile assetFile, IEnumerable<AssetData>? alreadyLoadedDependencies = null)
    {
        var assetType = Registries.AssetTypeRegistry.Get(assetFile.TypeName);
        if (assetType == null)
            throw new InvalidAssetTypeError(assetFile.TypeName);

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