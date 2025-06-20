namespace Engine.Asset;

using Data.RegistryManagers;

/// <summary>
/// Тип ассета
/// </summary>
/// <param name="Name">Имя типа</param>
/// <param name="AssetLoader">Объект загрузчика</param>
public sealed record AssetType(string Name, AssetLoader AssetLoader) : IDisposable
{
    public void Dispose()
    {
        CoreRegistries.AssetTypeRegistry.Pop(this);
        GC.SuppressFinalize(this);
    }

    ~AssetType() => Dispose();
}