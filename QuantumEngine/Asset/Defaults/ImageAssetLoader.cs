namespace QuantumCore.Asset.Defaults;

using Image;

public sealed class ImageAssetLoader : AssetLoader
{
    public override object LoadFile(
        AssetFile assetFile,
        IEnumerable<AssetData> dependencies
    )
    {
        return new Image(
            data: ReadBinaryAsset(assetFile),
            identifier: assetFile.Identifier
        );
    }
}