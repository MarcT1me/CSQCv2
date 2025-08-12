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
using Engine.Events.QuantumEvents.Window;
using Engine.Input.Keyboard;
using MirageAPI.Window;

namespace AppLib.Game;

[StructLayout(LayoutKind.Sequential)]
struct Vertex
{
    public Vector3 Position;
    public Vector2 UV;
}

[StructLayout(LayoutKind.Sequential)]
public struct MatrixBufferData
{
    public Matrix4 World;
    public Matrix4 View;
    public Matrix4 Projection;
}

public class GameWindow : Window
{
    // shader
    private DX12Shader _vertexShader = null!;
    private DX12Shader _pixelShader = null!;
    private DX12PipelineState _pipelineState = null!;

    // vertex
    private DX12VertexBuffer _vertexBuffer = null!;
    private int _vertexCount;

    // texture
    private DX12DescriptorHeap _descriptorHeap = null!;
    private DX12Texture _texture = null!;

    // 3d
    private DX12ConstantBuffer _matrixBuffer = null!;
    public GameCamera Camera = null!;

    public GameWindow(
        WinData winData,
        GlData? glData = null,
        string? name = null,
        Window? parent = null,
        NativeMonitorInfo? monitor = null
    )
        : base(winData, glData, name, parent, monitor)
    {
        LoadShaders();
        CreatePipelineState();
        LoadImage();
        CreateGeometryBuffers();
        CreateMatrixBuffer();
        CreateCamera();
    }

    public override void Prepare()
    {
        base.Prepare();
        
        NativeWindow.SetMouseVisibility(true);
        NativeWindow.SetMouseCapture(true);
    }

    private void LoadShaders()
    {
        Logger.Debug("Compiling shaders...");

        var vertShader = (ShaderData)AssetManager.Load(
            new(
                "vertexShader", "AppLib:Shaders/3dRenderShader.hlsl",
                isEmbedded: true, identifier: "Shader-Vert"
            )
        ).Content;
        _vertexShader = vertShader.GetNativeShader;

        var pixShader = (ShaderData)AssetManager.Load(
            new(
                "pixelShader", "AppLib:Shaders/3dRenderShader.hlsl",
                isEmbedded: true, identifier: "Shader-Pix"
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
                ),
                new DX12RootParameter(
                    DX12ResourceType.ConstantBuffer,
                    DX12ShaderVisibility.Vertex
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
        Logger.Debug("Creating Descriptor Heap...");
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

    private void CreateMatrixBuffer()
    {
        Logger.Debug("Creating matrix buffer...");
        _matrixBuffer = new DX12ConstantBuffer(256, DX12ResourceFlags.None);
    }

    private void CreateCamera()
    {
        Camera = new GameCamera(
            new(
                position: new Vector3(0, 0, 2),
                fov: 70,
                clipPlanes: new Vector2(0.001f, 100.0f),
                identifier: "GameCamera"
            )
        );
    }

    public override void HandleEvent(QuantumEvent e)
    {
        base.HandleEvent(e);
        switch (e)
        {
            case KeyEvent { Type: EventType.KeyDown, Key: Key.F11 }:
                Logger.Debug("ToggleFullscreen");
                NativeWindow.ToggleFullscreen();
                break;
            case WinResizeEvent winResize:
                Camera.SetAspectRatio(winResize.Size.X, winResize.Size.Y);
                break;
        }
    }

    public override void Update()
    {
        base.Update();

        Matrix4 world = Matrix4.Identity;

        MatrixBufferData matrixData = new MatrixBufferData
        {
            World = world,
            View = Camera.ViewMatrix,
            Projection = Camera.ProjectionMatrix
        };
        var bufferSize = (int)_matrixBuffer.Size;
        byte[] buffer = new byte[bufferSize];

        IntPtr ptr = Marshal.AllocHGlobal(bufferSize);
        Marshal.StructureToPtr(matrixData, ptr, false);
        Marshal.Copy(ptr, buffer, 0, bufferSize);
        Marshal.FreeHGlobal(ptr);
        _matrixBuffer.UploadData(buffer);
    }

    public override void Render()
    {
        base.Render();

        // Подготовка трубы и стыковка с CommandList
        var commandList = NativeWindow.DXContext.CmdList;

        // Финальная подготовка объекта к рендеру
        commandList.SetPrimitiveTopology(DX12PrimitiveTopology.TriangleList);
        commandList.BindBuffer(_vertexBuffer);
        commandList.BindBuffer(1, _matrixBuffer);

        // Рендер
        commandList.DrawInstanced((uint)_vertexCount, 1, 0, 0);
    }

    public override void Dispose()
    {
        _texture.ReleaseSRV();
        _texture.Dispose();
        _descriptorHeap.Dispose();

        _vertexBuffer.Dispose();
        _pipelineState.Dispose();
        _pixelShader.Dispose();
        _vertexShader.Dispose();
        base.Dispose();
    }
}