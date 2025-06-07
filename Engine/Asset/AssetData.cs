namespace Engine.Asset;

using Data;
using Data.Meta;

/// <summary>
/// Загруженный и упакованный ассет
/// </summary>
/// <param name="typeName">Имя типа</param>
/// <param name="content">Данные ассета</param>
/// <param name="dependencies">Зависимости ассета</param>
/// <param name="identifier">Уникальны идентификатор ассета</param>
public sealed class AssetData(
    string typeName,
    object content,
    IEnumerable<AssetData>? dependencies = null,
    Identifier? identifier = null
) : MetaData(identifier)
{
    public string Name { get; } = typeName;
    public IEnumerable<AssetData>? Dependencies { get; } = dependencies;
    public object Content = content;
}