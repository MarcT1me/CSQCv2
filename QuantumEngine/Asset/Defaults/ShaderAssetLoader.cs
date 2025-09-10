using System.Text;
using MirageAPI.DirectX.Shader;

namespace QuantumCore.Asset.Defaults;

using Shader;

public class VertexShaderAssetLoader : AssetLoader
{
    public override object LoadFile(
        AssetFile assetFile,
        IEnumerable<AssetData> dependencies
    )
    {
        return ShaderCacheManager.GetCachedShader(
            assetFile,
            Encoding.UTF8,
            new ShaderData(
                "VS",
                "vs_5_0",
                DX12ShaderType.Vertex,
                assetFile.GetFileName(),
                identifier: assetFile.Identifier
            ),
            DX12ShaderCompileFlags.None
        );
    }
}

public class PixelShaderAssetLoader : AssetLoader
{
    public override object LoadFile(
        AssetFile assetFile,
        IEnumerable<AssetData> dependencies
    )
    {
        return ShaderCacheManager.GetCachedShader(
            assetFile,
            Encoding.UTF8,
            new ShaderData(
                "PS",
                "ps_5_0",
                DX12ShaderType.Pixel,
                assetFile.GetFileName(),
                identifier: assetFile.Identifier
            ),
            DX12ShaderCompileFlags.None
        );
    }
}