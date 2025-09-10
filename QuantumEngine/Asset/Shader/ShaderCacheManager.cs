using System.Diagnostics;
using System.Text;
using MirageAPI.DirectX.Shader;
using QuantumCore.Logging;

namespace QuantumCore.Asset.Shader;

// Cache generator
public static class ShaderCacheManager
{
    private static string? _cacheDirectory;

    public static string CacheDirectory
    {
        get => _cacheDirectory ?? Path.Combine(
            Core.RootDirectory,
            "CachedShaders"
        );
        set => _cacheDirectory = value;
    }

    public static ShaderData GetCachedShader(
        AssetFile assetFile,
        Encoding encoding,
        ShaderData shaderData,
        DX12ShaderCompileFlags flags
    )
    {
        (var isSuccess, shaderData) = TryGetCachedShader(shaderData);
        if (isSuccess) return shaderData;

        shaderData.Source = AssetLoader.ReadTextAsset(assetFile, encoding);

        return CompileAndSave(shaderData, flags);
    }

    public static ShaderData GetCachedShader(
        ShaderData shaderData,
        DX12ShaderCompileFlags flags
    )
    {
        Debug.Assert(shaderData.Source == null, "ShaderData has empty Source");

        (var isSuccess, shaderData) = TryGetCachedShader(shaderData);
        if (isSuccess) return shaderData;

        return CompileAndSave(shaderData, flags);
    }

    public static (bool, ShaderData) TryGetCachedShader(ShaderData shaderData)
    {
        var cachePath = GetCachePath(shaderData);

        if (!File.Exists(cachePath) || Core.IsDebug) return (false, shaderData);

        Logger.Success($"Shader ({shaderData.Identifier}) cache file found!");

        shaderData.NativeShader = DX12Shader.LoadFromFile(cachePath, shaderData.Type);
        return (true, shaderData);
    }

    public static string GetCachePath(ShaderData shaderData)
    {
        string cacheFileName = $"{shaderData.MasterShaderName}.{shaderData.Entry}.{shaderData.Type}.cso";
        return Path.Combine(CacheDirectory, shaderData.MasterShaderName, cacheFileName);
    }

    public static ShaderData CompileAndSave(
        ShaderData shaderData,
        DX12ShaderCompileFlags flags
    )
    {
        DX12Shader shader = shaderData.NativeShader = DX12ShaderCompiler.CompileShaderFromSource(
            shaderData.Source,
            shaderData.Type,
            shaderData.Entry,
            shaderData.Target,
            flags
        );

        Logger.Success($"Shader ({shaderData.Identifier}) Compiled!");

        Directory.CreateDirectory(Path.Combine(CacheDirectory, shaderData.MasterShaderName));
        if (!Core.IsDebug)
        {
            shader.SaveToFile(GetCachePath(shaderData));
            Logger.Success($"Cache for Shader ({shaderData.Identifier}) saved!");
        }

        return shaderData;
    }
}