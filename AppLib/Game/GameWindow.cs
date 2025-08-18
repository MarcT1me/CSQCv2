using System.Runtime.InteropServices;
using Engine.Asset;
using Engine.Asset.Image;
using Engine.Asset.Shader;
using Engine.Events.QuantumEvents;
using Engine.Events.QuantumEvents.Mouse;
using Engine.Graphic.Window;
using Engine.Input.Keyboard;
using Engine.Logging;
using Engine.Time;
using MirageAPI;
using MirageAPI.DirectX;
using MirageAPI.DirectX.Command;
using MirageAPI.DirectX.Pipeline;
using MirageAPI.DirectX.Resource;
using MirageAPI.DirectX.Shader;
using OpenTK.Mathematics;
using Window = Engine.Graphic.Window.Window;

// MirageAPI
// Engine

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
        GlData glData,
        string name,
        DisplayInfo? display = null,
        IconInfo? icon = null,
        CursorInfo? cursor = null
    ) : base(
        winData, glData, name,
        icon: icon,
        display: display,
        cursor: cursor
    )
    {
        LoadShaders();
        CreatePipelineState();
        LoadImage();
        CreateGeometryBuffers();
        CreateMatrixBuffer();
        CreateCamera();
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

        float unitX = _texture.Width / 200f;
        float unitY = _texture.Height / 200f;

        Vertex[] vertices =
        [
            new()
            {
                Position = new Vector3(-unitX, 0.0f, unitY),
                UV = new Vector2(0, 0)
            },
            new()
            {
                Position = new Vector3(unitX, 0.0f, unitY),
                UV = new Vector2(1, 0)
            },
            new()
            {
                Position = new Vector3(-unitX, 0.0f, -unitY),
                UV = new Vector2(0, 1)
            },

            new()
            {
                Position = new Vector3(unitX, 0.0f, unitY),
                UV = new Vector2(1, 0)
            },
            new()
            {
                Position = new Vector3(unitX, 0.0f, -unitY),
                UV = new Vector2(1, 1)
            },
            new()
            {
                Position = new Vector3(-unitX, 0.0f, -unitY),
                UV = new Vector2(0, 1)
            }
        ];

        try
        {
            int vertexSize = Marshal.SizeOf<Vertex>();
            _vertexCount = vertices.Length;

            _vertexBuffer = new DX12VertexBuffer(
                (uint)_vertexCount,
                (uint)vertexSize,
                DX12ResourceFlags.None
            );

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
                position: (0, -10, 0),
                rotation: (0, 0, 90),
                identifier: "GameCamera"
            )
        );
    }

    protected override void Establish()
    {
        NativeWindow.IconMenu = new TrayIconMenu(NativeWindow, "ХУЙ меню", 1, null);
        NativeWindow.IconMenu.AddItem(
            new MenuItem(MenuItemType.String, 1, "ХУЙ"
            )
        );
        NativeWindow.IconMenu.AddItem(
            new MenuItem(MenuItemType.String, 2, "Вертай нахуй"
            )
        );
        NativeWindow.IconMenu.Callback += id =>
        {
            Logger.Info($"SysMenu: {id}");
            if (id == 2)
            {
                NativeWindow.ShowFromTray();
            }
        };

        base.Establish();
    }

    public override void HandleEvent(QuantumEvent e)
    {
        base.HandleEvent(e);

        if (e is MouseEvent { Type: EventType.MouseMove }) return;

        Logger.Info($"handle event: {e}");

        switch (e)
        {
            case KeyEvent { Type: EventType.KeyDown, Key: Key.F11 } keyEvent:
            {
                Logger.Debug("Toggle Fullscreen");
                ToggleFullscreen();

                if (keyEvent.Mods.HasFlag(KeyMod.Shift))
                {
                    Logger.Debug("set Mouse Capture and visibility");
                    NativeWindow.SetMouseCapture(MetaData.WinData.Fullscreen);
                    NativeWindow.SetMouseVisibility(!MetaData.WinData.Fullscreen);
                }

                break;
            }
            case KeyEvent { Type: EventType.KeyDown, Key: Key.F }:
            {
                NativeWindow.Flash(3, 1);
                Logger.Beep();
                break;
            }
            case KeyEvent { Type: EventType.KeyDown, Key: Key.F1 }:
            {
                NativeWindow.HideToTray();
                break;
            }
        }
    }

    public override void Update(ClockMeta clockMeta)
    {
        base.Update(clockMeta);

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

    public override void PreRender()
    {
        base.PreRender();
        TestApp.Instance.Scene.PreRender(MetaData);
    }

    public override void Render()
    {
        base.Render();

        TestApp.Instance.Scene.Render(MetaData);

        // Подготовка трубы и стыковка с CommandList
        var commandList = NativeWindow.DXContext.CmdList;

        // Финальная подготовка объекта к рендеру
        commandList.SetPrimitiveTopology(DX12PrimitiveTopology.TriangleList);
        commandList.BindBuffer(_vertexBuffer);
        commandList.BindBuffer(1, _matrixBuffer);

        // Рендер
        commandList.DrawInstanced((uint)_vertexCount, 1, 0, 0);
    }

    public override void PostRender()
    {
        TestApp.Instance.Scene.PostRender(MetaData);

        base.PostRender();
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