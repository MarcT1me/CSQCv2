namespace Engine.Asset;

/// <summary>
/// Тип ассета
/// </summary>
/// <param name="Name">Имя типа</param>
/// <param name="AssetLoader">Объект загрузчика</param>
public sealed record AssetType(string Name, AssetLoader AssetLoader);