using System.Runtime.InteropServices;
using Engine.Asset;
using Engine.Events.QuantumEvents;
using OpenTK.Mathematics;
using Engine.Graphic.Window;
using Engine.Logging;
using MirageAPI.DirectX;
using System.Reflection;
using System.Text;
using Engine;

namespace AppLib.Game;

public static class ShaderResourceLoader
{
    public static string LoadEmbeddedShader(string resourceName)
    {
        var assembly = Assembly.GetExecutingAssembly();
        using var stream = assembly.GetManifestResourceStream(resourceName);
        if (stream == null)
            throw new FileNotFoundException($"Embedded shader resource not found: {resourceName}");

        using var reader = new StreamReader(stream, Encoding.UTF8);
        return reader.ReadToEnd();
    }

    public static byte[] LoadEmbeddedShaderBytes(string resourceName)
    {
        var assembly = Assembly.GetExecutingAssembly();
        using var stream = assembly.GetManifestResourceStream(resourceName);
        if (stream == null)
            throw new FileNotFoundException($"Embedded shader resource not found: {resourceName}");

        using var ms = new MemoryStream();
        stream.CopyTo(ms);
        return ms.ToArray();
    }
}

public static class ShaderCacheManager
{
    private static string CacheDirectory => Path.Combine(
        EngineCore.RootDirectory,
        "ShaderCache"
    );

    public static unsafe DX12Shader GetCachedShader(
        string shaderName,
        DX12ShaderType type,
        string entryPoint,
        string target,
        DX12ShaderCompileFlags flags0,
        DX12ShaderCompileFlags flags1,
        string? hlslSource = null)
    {
        Directory.CreateDirectory(CacheDirectory);

        string cacheFileName = $"{shaderName}_{type}_{entryPoint}.cso";
        string cachePath = Path.Combine(CacheDirectory, cacheFileName);

        if (File.Exists(cachePath))
        {
            return DX12Shader.LoadFromFile(cachePath, type);
        }

        DX12Shader shader;
        if (hlslSource != null)
        {
            shader = DX12ShaderCompiler.CompileShaderFromSource(
                hlslSource,
                type,
                entryPoint,
                target,
                flags0,
                flags1,
                null
            );
        }
        else
        {
            string shaderPath = Path.Combine(
                AssetLoader.AssetDirectory,
                "Shaders",
                $"{shaderName}.hlsl"
            );

            shader = DX12ShaderCompiler.CompileShaderFromFile(
                shaderPath,
                type,
                entryPoint,
                target,
                flags0,
                flags1,
                null
            );
        }

        // Сохраняем в кеш
        shader.SaveToFile(cachePath);
        return shader;
    }
}

[StructLayout(LayoutKind.Sequential)]
struct Vertex
{
    public Vector3 Position;
}

public class GameWindow : Window
{
    private DX12PipelineState _pipelineState = null!;
    private DX12VertexBuffer _vertexBuffer = null!;
    private DX12Shader _vertexShader = null!;
    private DX12Shader _pixelShader = null!;

    public GameWindow(WinData winData, GlData? glData = null, string? name = null)
        : base(winData, glData, name)
    {
        LoadShaders();
        CreatePipelineState();
        CreateGeometryBuffers();
    }

    public override void Prepare()
    {
        NativeWindow.ToggleFullscreen();
        base.Prepare();
    }

    private void LoadShaders()
    {
        const string shaderName = "SimpleShader";
        const string vsEntry = "VS";
        const string psEntry = "PS";
        const string vsTarget = "vs_5_0";
        const string psTarget = "ps_5_0";

#if DEBUG
        var shaderFlags = DX12ShaderCompileFlags.Debug | DX12ShaderCompileFlags.SkipOptimization;

        string hlslSource = ShaderResourceLoader.LoadEmbeddedShader(
            $"AppLib.Assets.Shaders.{shaderName}"
        );

        unsafe
        {
            _vertexShader = DX12ShaderCompiler.CompileShaderFromSource(
                hlslSource,
                DX12ShaderType.Vertex,
                vsEntry,
                vsTarget,
                shaderFlags,
                DX12ShaderCompileFlags.None,
                null
            );

            _pixelShader = DX12ShaderCompiler.CompileShaderFromSource(
                hlslSource,
                DX12ShaderType.Pixel,
                psEntry,
                psTarget,
                shaderFlags,
                DX12ShaderCompileFlags.None,
                null
            );
        }
#else
        var shaderFlags = DX12ShaderCompileFlags.OptimizationLevel3;

        string hlslSource = ShaderResourceLoader.LoadEmbeddedShader(
            $"AppLib.Assets.Shaders.{shaderName}"
        );

        _vertexShader = ShaderCacheManager.GetCachedShader(
            shaderName,
            DX12ShaderType.Vertex,
            vsEntry,
            vsTarget,
            shaderFlags,
            DX12ShaderCompileFlags.None,
            hlslSource
        );

        _pixelShader = ShaderCacheManager.GetCachedShader(
            shaderName,
            DX12ShaderType.Pixel,
            psEntry,
            psTarget,
            shaderFlags,
            DX12ShaderCompileFlags.None,
            hlslSource
        );
#endif
    }

    private void CreatePipelineState()
    {
        _pipelineState = new DX12PipelineState(_vertexShader, _pixelShader, DX12BufferFormat.RGBA_UNORM);
    }

    private void CreateGeometryBuffers()
    {
        Vertex[] vertices =
        [
            new() { Position = new Vector3(-1.0f, 1.0f, 0.0f) },
            new() { Position = new Vector3(1.0f, 1.0f, 0.0f) },
            new() { Position = new Vector3(-1.0f, -1.0f, 0.0f) },

            new() { Position = new Vector3(1.0f, 1.0f, 0.0f) },
            new() { Position = new Vector3(1.0f, -1.0f, 0.0f) },
            new() { Position = new Vector3(-1.0f, -1.0f, 0.0f) }
        ];

        // Создаем вершинный буфер
        int vertexSize = Marshal.SizeOf<Vertex>();
        int bufferSize = vertices.Length * vertexSize;

        _vertexBuffer = new DX12VertexBuffer((uint)bufferSize, (uint)vertexSize);
        try
        {
            unsafe
            {
                fixed (Vertex* verticesPtr = vertices)
                {
                    byte[] vertexData = new byte[bufferSize];
                    Marshal.Copy((IntPtr)verticesPtr, vertexData, 0, bufferSize);
                    _vertexBuffer.UploadData(vertexData);
                }
            }
        }
        catch
        {
            _vertexBuffer.Dispose();
            throw;
        }
    }

    public override void HandleEvent(QuantumEvent e)
    {
        base.HandleEvent(e);
        if (e is KeyEvent { Type: EventType.KeyDown, Key: 122 })
        {
            Logger.Debug("ToggleFullscreen");
            NativeWindow.ToggleFullscreen();
        }
    }

    public override void Render()
    {
        base.Render();

        var commandList = NativeWindow.DXContext.CommandList;
        commandList.SetPipelineState(_pipelineState);
        commandList.SetGraphicsRootSignature(_pipelineState);

        commandList.IASetPrimitiveTopology(PrimitiveTopology.TriangleList);

        commandList.IASetVertexBuffer(_vertexBuffer);

        commandList.DrawInstanced(6, 1, 0, 0);
    }

    public override void Dispose()
    {
        _vertexBuffer.Dispose();
        _pipelineState.Dispose();
        _pixelShader.Dispose();
        _vertexShader.Dispose();
        base.Dispose();
    }
}