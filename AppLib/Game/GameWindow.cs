using System.Runtime.InteropServices;
using Engine.Events.QuantumEvents;
using OpenTK.Mathematics;
using Engine.Graphic.Window;
using Engine.Logging;
using MirageAPI.DirectX;
using MirageAPI.DirectX.Shader;
using MirageAPI.DirectX.Pipeline;
using MirageAPI.DirectX.Resource;
using System.Reflection;
using System.Text;
using Engine.Asset;
using Engine.Asset.Defaults;

namespace AppLib.Game;

public static class EmbedResourceLoader
{
    public static string LoadEmbeddedText(string resourceName, Encoding encoding)
    {
        var assembly = Assembly.GetExecutingAssembly();
        using var stream = assembly.GetManifestResourceStream(resourceName);
        if (stream == null)
            throw new FileNotFoundException($"Embedded shader resource not found: {resourceName}");

        using var reader = new StreamReader(stream, encoding);
        return reader.ReadToEnd();
    }
}

// Cache generator
#if !DEBUG
public static class ShaderCacheManager
{
    private static string CacheDirectory => Path.Combine(
        AssetLoader.AssetDirectory,
        "Shaders"
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
        Directory.CreateDirectory(Path.Combine(CacheDirectory, shaderName));

        string cacheFileName = $"{shaderName}_{type}_{entryPoint}.cso";
        string cachePath = Path.Combine(CacheDirectory, shaderName, cacheFileName);

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
#endif

[StructLayout(LayoutKind.Sequential)]
struct Vertex
{
    public Vector3 Position;
    public Vector2 UV;
}

public class GameWindow : Window
{
    private DX12Shader _vertexShader = null!;
    private DX12Shader _pixelShader = null!;
    private DX12PipelineState _pipelineState = null!;
    private DX12VertexBuffer _vertexBuffer = null!;
    private int _vertexCount;

    private DX12DescriptorHeap? _descriptorHeap;
    private AssetData? _img;
    private DX12Texture? _texture;

    public GameWindow(WinData winData, GlData? glData = null, string? name = null)
        : base(winData, glData, name)
    {
        CreatePipelineState();
        LoadImage();
        CreateGeometryBuffers();
    }

    private void CreatePipelineState()
    {
        LoadShaders();

        Logger.Debug("Creating pipeline...");

        // Создание конфигураций
        var config = DX12PipelineStateConfig.Default;
        // Вершинный и пиксельный шейдер
        config.VertexShader = _vertexShader;
        config.PixelShader = _pixelShader;
        // Данные вершин
        config.InputLayouts =
        [
            new DX12InputElement
            {
                SemanticName = "POSITION",
                Format = DX12ResourceFormat.RGB32_FLOAT,
                Offset = 0
            },
            new DX12InputElement
            {
                SemanticName = "TEXCOORD",
                Format = DX12ResourceFormat.RG32_FLOAT,
                Offset = 12
            }
        ];
        // Параметры и буферы шейдеров
        config.RootParams =
        [
            new DX12PipelineParameter
            {
                Type = DX12ResourceType.Texture,
                RegisterSlot = 0,
                RegisterSpace = 0
            }
        ];
        // Создания трубы
        _pipelineState = new DX12PipelineState(config);
        NativeWindow.DXContext.CmdList.PipelineState = _pipelineState;
    }

    private void LoadShaders()
    {
        const string shaderName = "TextureShader";
        const string vsEntry = "VS";
        const string psEntry = "PS";
        const string vsTarget = "vs_5_0";
        const string psTarget = "ps_5_0";

        Logger.Debug("Compiling shaders...");

#if DEBUG
        // Загружаем текстуру в дебаг-режиме (без кеширования)
        var shaderFlags = DX12ShaderCompileFlags.Debug | DX12ShaderCompileFlags.SkipOptimization;

        string hlslSource = EmbedResourceLoader.LoadEmbeddedText(
            $"AppLib.Assets.Shaders.{shaderName}",
            Encoding.UTF8
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
        // Загрузка шейдеров в релизе (предварительное кеширование для первого раза, потом просто использование)
        var shaderFlags = DX12ShaderCompileFlags.OptimizationLevel3;

        string hlslSource = EmbedResourceLoader.LoadEmbeddedText(
            $"AppLib.Assets.Shaders.{shaderName}",
            Encoding.UTF8
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

    private void LoadImage()
    {
        Logger.Debug("Loading image...");

        // Загружаем ассет текстуры
        _img = AssetManager.Load(new("image", "IMG.png"));
        ImageData imageData = (ImageData)_img.Content;

        // Создаём текстуру
        _texture = new DX12Texture(DX12ResourceConfig.TextureConfig(
            (uint)imageData.Size.X,
            (uint)imageData.Size.Y,
            DX12ResourceFormat.RGBA8_UNORM,
            1,
            1,
            DX12TextureType.Texture2D,
            DX12ResourceFlags.None
        ));

        // Загружаем данные и выделяем память в куче
        _texture.UploadData(ConvertToAlpha(imageData.Data));
        NativeWindow.DXContext.CmdList.DescriptorHeap = _texture.SRVHeap = CreateDescriptorHeap();
        _texture.CreateSRV();
    }

    private byte[] ConvertToAlpha(byte[] rgbData)
    {
        int newSize = rgbData.Length / 3 * 4;
        byte[] rgbaData = new byte[newSize];

        for (int i = 0, j = 0; i < rgbData.Length; i += 3, j += 4)
        {
            rgbaData[j] = rgbData[i];
            rgbaData[j + 1] = rgbData[i + 1];
            rgbaData[j + 2] = rgbData[i + 2];
            rgbaData[j + 3] = 255;
        }

        return rgbaData;
    }

    private byte[] ConvertToAlphaBroken(byte[] rgbData)
    {
        // Вычисляем размер (добавляем компоненту A)
        int newSize = rgbData.Length / 3 * 4;

        // Создаём новый буфер и копируем данные
        byte[] rgbaData = new byte[newSize];
        for (int i = 0; i < rgbData.Length; i += 4)
        {
            rgbaData[i] = rgbData[i];
            rgbaData[i + 1] = rgbData[i + 1];
            rgbaData[i + 2] = rgbData[i + 2];
            rgbaData[i + 3] = 255;
        }

        return rgbaData;
    }

    private DX12DescriptorHeap CreateDescriptorHeap()
    {
        return _descriptorHeap = new DX12DescriptorHeap(DX12DescriptorHeapType.CBV_SRV_UAV, 1, true);
    }

    private void CreateGeometryBuffers()
    {
        Logger.Debug("Creating geometry buffer...");

        // Создаем вершинны
        Vertex[] vertices =
        [
            new()
            {
                Position = new Vector3(-1.0f, 1.0f, 0.0f),
                UV = new Vector2(0, 0)
            },
            new()
            {
                Position = new Vector3(1.0f, 1.0f, 0.0f),
                UV = new Vector2(1, 0)
            },
            new()
            {
                Position = new Vector3(-1.0f, -1.0f, 0.0f),
                UV = new Vector2(0, 1)
            },

            new()
            {
                Position = new Vector3(1.0f, 1.0f, 0.0f),
                UV = new Vector2(1, 0)
            },
            new()
            {
                Position = new Vector3(1.0f, -1.0f, 0.0f),
                UV = new Vector2(1, 1)
            },
            new()
            {
                Position = new Vector3(-1.0f, -1.0f, 0.0f),
                UV = new Vector2(0, 1)
            }
        ];

        // Создаем вершинный буфер
        int vertexSize = Marshal.SizeOf<Vertex>();
        _vertexCount = vertices.Length;

        _vertexBuffer = new DX12VertexBuffer((uint)_vertexCount, (uint)vertexSize);

        try
        {
            unsafe
            {
                fixed (Vertex* verticesPtr = vertices)
                {
                    int bufferSize = _vertexCount * vertexSize;
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

    public override void Prepare()
    {
        Logger.Debug("Preparing window...");

        base.Prepare();
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

        // Подготовка трубы и стыковка с CommandList
        var commandList = NativeWindow.DXContext.CmdList;

        // Финальная подготовка объекта к рендеру
        commandList.SetPrimitiveTopology(DX12PrimitiveTopology.TriangleList);
        commandList.BindBuffer(_vertexBuffer);

        // Рендер
        commandList.DrawInstanced((uint)_vertexCount, 1, 0, 0);
    }

    public override void Dispose()
    {
        _texture?.ReleaseSRV();
        _texture?.Dispose();
        _descriptorHeap?.Dispose();
        _vertexBuffer.Dispose();
        _pipelineState.Dispose();
        _pixelShader.Dispose();
        _vertexShader.Dispose();
        base.Dispose();
    }
}