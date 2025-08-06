using OpenTK.Mathematics;

namespace Engine.Asset.Image;

using Data;
using Data.Meta;

public class ImageData(
    Vector2i size,
    ImageFormat format,
    byte[] data,
    Identifier? identifier = null
)
    : MetaData(identifier)
{
    public Vector2i Size { get; } = size;
    public ImageFormat Format { get; } = format;
    public byte[] Data { get; } = data;
}