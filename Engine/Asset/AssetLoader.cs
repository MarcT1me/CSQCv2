using Engine.Configuration;

namespace Engine.Asset;

public abstract class AssetLoader
{
    public abstract object LoadFile(AssetFile assetFile);

    public abstract AssetData CreateAsset(
        AssetFile assetFile,
        IEnumerable<AssetData>? dependencies,
        object content
    );

    protected static string ReadTextFile(string path)
    {
        return File.ReadAllText(path);
    }

    protected static byte[] ReadBinaryFile(string path)
    {
        return File.ReadAllBytes(path);
    }

    protected static Stream OpenFile(string path)
    {
        return File.OpenRead(path);
    }

    public static string AssetDirectory => Path.Combine(
        EngineCore.RootDirectory,
        BaseConfig.AssetPath
    );
}