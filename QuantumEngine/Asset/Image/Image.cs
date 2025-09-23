using MirageAPI.DirectX;
using MirageAPI.DirectX.Resource;
using MirageAPI.DirectX.Shader;
using OpenTK.Mathematics;
using StbImageSharp;

namespace QuantumCore.Asset.Image;

using Failures;
using Data;
using Data.Meta;

public class Image : MetaObject<ImageData>
{
    public DX12Texture GetNativeTexture(
        DX12ResourceFlags flags = DX12ResourceFlags.None,
        uint depth = 1, uint mipLevels = 1,
        DX12TextureType textureType = DX12TextureType.Texture2D,
        DX12ViewDimension dimension = DX12ViewDimension.Texture2D
    )
    {
        // convert if it needs
        Image operatedImg = MetaData.Format == ImageFormat.RGB ? ConvertToAlpha() : this;

        // create native resource
        DX12Texture texture = new DX12Texture(
            DX12ResourceConfig.TextureConfig(
                Id, DX12ShaderVisibility.All,
                (uint)operatedImg.MetaData.Size.X,
                (uint)operatedImg.MetaData.Size.Y,
                operatedImg.NativeFormat, flags,
                depth, mipLevels,
                textureType, dimension
            )
        );
        // upload data and return gotten texture
        texture.UploadData(operatedImg.MetaData.Data);
        return texture;
    }

    public DX12ResourceFormat NativeFormat => MetaData.Format switch
    {
        ImageFormat.Grey => DX12ResourceFormat.R8_UNORM,
        ImageFormat.GreyAlpha => DX12ResourceFormat.RG8_UNORM,
        ImageFormat.RGB or ImageFormat.RGBA => DX12ResourceFormat.RGBA8_UNORM,
        _ => throw new FailureException($"Invalid image format {MetaData.Format}")
    };

    public Image(byte[] data, Identifier? identifier = null)
        : this(ImageResult.FromMemory(data), identifier)
    {
    }

    public Image(Stream stream, Identifier? identifier = null)
        : this(ImageResult.FromStream(stream), identifier)
    {
    }

    private Image(ImageResult imgData, Identifier? identifier = null)
        : this(imgData.Data, new Vector2i(imgData.Width, imgData.Height), (ImageFormat)imgData.SourceComp, identifier)
    {
    }

    private Image(byte[] data, Vector2i size, ImageFormat format, Identifier? identifier = null)
        : base(new ImageData(size, format, data, identifier))
    {
    }

    public Image ConvertToAlpha(byte fillValue = 255)
    {
        switch (MetaData.Format)
        {
            case ImageFormat.RGB:
            {
                int newSize = MetaData.Data.Length / 3 * 4;
                byte[] newData = new byte[newSize];

                for (int i = 0, j = 0; i < MetaData.Data.Length; i += 3, j += 4)
                {
                    newData[j] = MetaData.Data[i];
                    newData[j + 1] = MetaData.Data[i + 1];
                    newData[j + 2] = MetaData.Data[i + 2];
                    newData[j + 3] = fillValue;
                }

                return new Image(newData, MetaData.Size, ImageFormat.RGBA);
            }
            case ImageFormat.Grey:
            {
                int newSize = MetaData.Data.Length * 2;
                byte[] newData = new byte[newSize];

                for (int i = 0, j = 0; i < MetaData.Data.Length; i += 1, j += 2)
                {
                    newData[j] = MetaData.Data[i];
                    newData[j + 1] = fillValue;
                }

                return new Image(newData, MetaData.Size, ImageFormat.GreyAlpha);
            }
            default:
                throw new FailureException($"Cant convert image {this} to alpha");
        }
    }

    public override string ToString()
    {
        return $"Image<{Id}>(Size: {MetaData.Size}, Format: {MetaData.Format}, Data: {MetaData.Data.Length})";
    }
}