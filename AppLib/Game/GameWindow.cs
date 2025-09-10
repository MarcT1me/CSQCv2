using System.Runtime.InteropServices;
using QuantumCore.Asset;
using QuantumCore.Asset.Image;
using QuantumCore.Asset.Shader;
using QuantumCore.Events.QuantumEvents;
using QuantumCore.Events.QuantumEvents.Mouse;
using QuantumCore.Graphic.Window;
using QuantumCore.Input.Keyboard;
using QuantumCore.Logging;
using QuantumCore.Time;
using QuantumCore.UI;
using Microsoft.Toolkit.Uwp.Notifications;
using MirageAPI;
using MirageAPI.DirectX;
using MirageAPI.DirectX.Command;
using MirageAPI.DirectX.Pipeline;
using MirageAPI.DirectX.Resource;
using MirageAPI.DirectX.Shader;
using OpenTK.Mathematics;

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

public class TestNotification() : ToastNotification(DateTime.Now)
{
    public override void Build(ToastContentBuilder builder)
    {
        builder
            .AddText("TestNotification");
    }
}

public class GameWindow : QuantumCore.Graphic.Window.Window
{
    // shader
    private readonly DX12Shader _vertexShader;
    private readonly DX12Shader _pixelShader;
    private readonly DX12PipelineState _pipelineState;

    // vertex
    private readonly DX12VertexBuffer _vertexBuffer;
    private readonly int _vertexCount;

    // texture
    private readonly DX12DescriptorHeap _descriptorHeap;
    private readonly DX12Texture _texture;

    // 3d
    private readonly DX12ConstantBuffer _matrixBuffer;
    public GameCamera Camera;

    public GameWindow(
        WinData winData,
        GlData glData,
        string name,
        DisplayInfo? display = null
    ) : base(
        winData, glData, name,
        display: display
    )
    {
        Logger.Debug("Loading Shaders...");

        var vertShader = (ShaderData)AssetManager.Load(
            new(
                "vertexShader", "AppLib:Shaders/3dRenderShader.hlsl",
                identifier: "Shader-Vert"
            )
        ).Content;
        _vertexShader = vertShader.GetNativeShader;

        var pixShader = (ShaderData)AssetManager.Load(
            new(
                "pixelShader", "AppLib:Shaders/3dRenderShader.hlsl",
                identifier: "Shader-Pix"
            )
        ).Content;
        _pixelShader = pixShader.GetNativeShader;

        Logger.Debug("Creating Pipeline...");

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

        Logger.Debug("Loading Image...");

        // Загружаем ассет текстуры
        Image img = (Image)AssetManager.Load(
            new(
                "image", "AppLib:IMG.png",
                identifier: "IMG-Image"
            )
        ).Content;

        // Создаём текстуру
        _texture = img.GetNativeTexture();

        Logger.Debug("Creating Descriptor Heap...");
        _texture.SRVHeap = NativeWindow.DXContext.CmdList.DescriptorHeap =
            _descriptorHeap =
                new DX12DescriptorHeap(DX12DescriptorHeapType.CBV_SRV_UAV, 1, true);

        _texture.CreateSRV();

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
            _vertexBuffer?.Dispose();
            throw;
        }

        Logger.Debug("Creating matrix Constant Buffer...");
        _matrixBuffer = new DX12ConstantBuffer(256, DX12ResourceFlags.None);

        Logger.Debug("Creating Camera...");
        Camera = new GameCamera(
            new(
                position: (0, -10, 0),
                rotation: (0, 0, 90),
                identifier: "GameCamera"
            )
        );

        Logger.Debug("Creating Tray icon menu...");
        NativeWindow.TrayMenu = new TrayMenu(
            NativeWindow, 1, null,
            "Test Tray icon", "info", "idk what info"
        );
        var empty = NativeWindow.TrayMenu.TextItem("Empty");
        var pop = NativeWindow.TrayMenu.TextItem("Pop from tray");
        NativeWindow.TrayMenu.Callback += id =>
        {
            if (id == empty)
            {
                Logger.Debug("crickets chirping...");
            }

            if (id == pop)
            {
                Logger.Debug("removing tray icon...");
                NativeWindow.ShowFromTray();
            }
        };

        Logger.Debug("Changing System window menu...");
        NativeWindow.SysMenu.Separator();
        NativeWindow.SysMenu.TextItem("TEST SYSTEM MENU ITEM");
        NativeWindow.SysMenu.Callback += id => { Logger.Debug($"crickets chirping... {id}"); };
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
                IsFullscreen = !IsFullscreen;

                if (keyEvent.Mods.HasFlag(KeyMod.Shift))
                {
                    Logger.Debug("set Mouse Capture and visibility");
                    Mouse.CaptureWindow = IsFullscreen ? NativeWindow : null;
                    Mouse.IsVisible = !IsFullscreen;
                }

                break;
            }
            case KeyEvent { Type: EventType.KeyDown, Key: Key.N }:
            {
                new TestNotification()
                    .Show();
                break;
            }
            case KeyEvent { Type: EventType.KeyDown, Key: Key.Tilda }:
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
            case KeyEvent { Type: EventType.KeyUp, Key: Key.F2 }:
            {
                Mouse.IsVisible = !Mouse.IsVisible;
                break;
            }
            case KeyEvent { Type: EventType.KeyUp, Key: Key.F3 }:
            {
                Mouse.CaptureWindow = NativeWindow.Mouse.IsCapture ? null : NativeWindow;
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