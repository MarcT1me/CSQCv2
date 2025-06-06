namespace Engine.Asset;

using Data;
using Data.Meta;

public sealed class AssetFile(
    string typeName,
    string path,
    IEnumerable<AssetFile>? dependencies = null,
    Identifier? identifier = null
) : MetaData(identifier)
{
    public string TypeName { get; } = typeName;
    public string Path { get; } = path;
    public List<AssetFile> Dependencies { get; } = dependencies != null ? [..dependencies] : [];

    public string GetFullPath()
    {
        return System.IO.Path.Combine(
            AssetLoader.AssetDirectory,
            Path
        );
    }
}