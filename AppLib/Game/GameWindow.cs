using System.Runtime.InteropServices;
using OpenTK.Mathematics;
// MirageAPI
using MirageAPI.DirectX;
using MirageAPI.DirectX.Shader;
using MirageAPI.DirectX.Pipeline;
using MirageAPI.DirectX.Resource;
using MirageAPI.DirectX.Command;
// Engine
using Engine.Asset;
using Engine.Asset.Image;
using Engine.Asset.Shader;
using Engine.Graphic.Window;
using Engine.Logging;
using Engine.Events.QuantumEvents;

namespace AppLib.Game;

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
    private DX12Texture? _texture;

    public GameWindow(WinData winData, GlData? glData = null, string? name = null)
        : base(winData, glData, name)
    {
        LoadShaders();
        CreatePipelineState();
        LoadImage();
        CreateGeometryBuffers();
    }

    private void LoadShaders()
    {
        Logger.Debug("Compiling shaders...");

        var vertShader = (ShaderData)AssetManager.Load(
            new(
                "vertexShader", "AppLib:Shaders/TextureShader.hlsl",
                isEmbedded: true, identifier: "TextureShader-Vert"
            )
        ).Content;
        _vertexShader = vertShader.GetNativeShader;

        var pixShader = (ShaderData)AssetManager.Load(
            new(
                "pixelShader", "AppLib:Shaders/TextureShader.hlsl", 
                isEmbedded: true, identifier: "TextureShader-Pix"
            )
        ).Content;
        _pixelShader = pixShader.GetNativeShader;
    }

    private void CreatePipelineState()
    {
        Logger.Debug("Creating pipeline...");

        // Создание конфигураций
        var config = new DX12PipelineStateConfig(
            [
                new DX12RootParameter(
                    DX12ResourceType.Texture,
                    DX12ShaderVisibility.Pixel
                )
            ],
            [
                new DX12SamplerConfig(
                    0,
                    DX12ShaderVisibility.Pixel
                )
            ],
            new DX12RasterizerConfig(),
            new DX12BlendConfig(true, false),
            [
                new DX12InputElement(
                    "POSITION",
                    DX12ResourceFormat.RGB32_FLOAT,
                    0
                ),
                new DX12InputElement
                (
                    "TEXCOORD",
                    DX12ResourceFormat.RG32_FLOAT,
                    12
                )
            ]
        );
        NativeWindow.DXContext.CmdList.PipelineState =
            _pipelineState = new DX12PipelineState(_vertexShader, _pixelShader, config);
    }

    private void LoadImage()
    {
        Logger.Debug("Loading image...");

        // Загружаем ассет текстуры
        Image img = (Image)AssetManager.Load(
            new(
                "image", "AppLib:IMG.png",
                isEmbedded: true, identifier: "IMG-Image"
                )
        ).Content;

        // Создаём текстуру
        _texture = img.GetNativeTexture();
        _texture.SRVHeap = CreateDescriptorHeap();
        _texture.CreateSRV();
    }

    private DX12DescriptorHeap CreateDescriptorHeap()
    {
        return
            NativeWindow.DXContext.CmdList.DescriptorHeap =
                _descriptorHeap =
                    new DX12DescriptorHeap(DX12DescriptorHeapType.CBV_SRV_UAV, 1, true);
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

        _vertexBuffer = new DX12VertexBuffer((uint)_vertexCount, (uint)vertexSize, DX12ResourceFlags.None);

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