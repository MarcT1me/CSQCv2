using MirageAPI.DirectX.Shader;

namespace QuantumCore.Asset.Shader;

using Data;
using Data.Meta;

public class ShaderData(
    string entry,
    string target,
    DX12ShaderType type,
    string masterShaderName,
    string? source = null,
    DX12Shader? nativeShader = null,
    Identifier? identifier = null
) : MetaData(identifier)
{
    public string Entry { get; } = entry;
    public string Target { get; } = target;
    public DX12ShaderType Type { get; } = type;
    public string MasterShaderName { get; } = masterShaderName;

    public string? Source { get; internal set; } = source;

    internal DX12Shader? NativeShader = nativeShader;
    public DX12Shader GetNativeShader => NativeShader ?? throw new AssetError("ShaderData not have a native shader");
}