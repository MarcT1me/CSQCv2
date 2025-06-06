namespace Engine.Asset;

using Data;
using Data.Meta;

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