using OpenTK.Mathematics;
using StbImageSharp;

namespace Engine.Asset.Defaults;

public readonly struct ImageData
{
    private readonly ImageResult _imageResult;

    public byte[] Data => _imageResult.Data;
    public Vector2i Size => new Vector2i(_imageResult.Width, _imageResult.Height);
    public ColorComponents SourceComp => _imageResult.SourceComp;

    public ImageData(byte[] data)
        : this(ImageResult.FromMemory(data))
    {
    }

    [Obsolete("Stream using not safe in the AssetLoader. Use ImageData(byte[])")]
    public ImageData(Stream stream)
        : this(ImageResult.FromStream(stream, ColorComponents.RedGreenBlueAlpha))
    {
    }

    private ImageData(ImageResult imageResult)
    {
        _imageResult = imageResult;
    }
}

public sealed class ImageAssetLoader : AssetLoader
{
    public override object LoadFile(
        AssetFile assetFile
    )
    {
        return new ImageData(
            ReadBinaryFile(assetFile.GetFullPath())
        );
    }
}