using System.Runtime.InteropServices;
using Engine.Asset;
using OpenTK.Mathematics;
using Engine.Graphic.Window;
using MirageAPI.DirectX;

namespace AppLib.Game;

[StructLayout(LayoutKind.Sequential)]
struct Vertex
{
    public Vector3 Position;
    public Vector4 Color;
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

    private void LoadShaders()
    {
        _vertexShader = LoadShader(DX12ShaderType.Vertex, "Shaders\\SimpleShaderVS.cso");
        _pixelShader = LoadShader(DX12ShaderType.Pixel, "Shaders\\SimpleShaderPS.cso");
    }

    private DX12Shader LoadShader(DX12ShaderType type, string loadShader)
    {
        var name = AssetLoader.AssetDirectory + "\\" + loadShader;
        
        using var vsStream = File.OpenRead(name);
        if (vsStream == null)
            throw new FileNotFoundException($"Shader \"{loadShader}\" not found.");
        
        using var ms = new MemoryStream();
        vsStream.CopyTo(ms);
        return new DX12Shader(ms.ToArray(), type);
    }

    private void CreatePipelineState()
    {
        _pipelineState = new DX12PipelineState(_vertexShader, _pixelShader, DX12BufferFormat.RGBA_UNORM);
    }

    private void CreateGeometryBuffers()
    {
        Vertex[] vertices =
        [
            new()
            {
                Position = new Vector3(0.0f, 0.5f, 0.0f),
                Color = new Vector4(1.0f, 0.0f, 0.0f, 1.0f)
            },
            new()
            {
                Position = new Vector3(0.5f, -0.5f, 0.0f),
                Color = new Vector4(0.0f, 1.0f, 0.0f, 1.0f)
            },
            new()
            {
                Position = new Vector3(-0.5f, -0.5f, 0.0f),
                Color = new Vector4(0.0f, 0.0f, 1.0f, 1.0f)
            }
        ];
        
        // Создаем вершинный буфер
        int vertexSize = Marshal.SizeOf<Vertex>();
        int bufferSize = vertices.Length * vertexSize;
        
        _vertexBuffer = new DX12VertexBuffer((uint)bufferSize, (uint)vertexSize);
        
        byte[] vertexData = new byte[bufferSize];
        IntPtr ptr = Marshal.AllocHGlobal(bufferSize);
        
        for (int i = 0; i < vertices.Length; i++)
        {
            Marshal.StructureToPtr(vertices[i], ptr + i * vertexSize, false);
        }
        
        Marshal.Copy(ptr, vertexData, 0, bufferSize);
        Marshal.FreeHGlobal(ptr);
        
        _vertexBuffer.UploadData(vertexData);
    }

    public override void Render()
    {
        base.Render();

        var commandList = NativeWindow.DXContext.CommandList;
        commandList.SetPipelineState(_pipelineState);
        commandList.SetGraphicsRootSignature(_pipelineState);
        
        commandList.IASetPrimitiveTopology(PrimitiveTopology.TriangleList);
        
        commandList.IASetVertexBuffer(_vertexBuffer);
        
        commandList.DrawInstanced(3, 1, 0, 0);
    }

    public override void Dispose()
    {
        _vertexBuffer.Dispose();
        _pipelineState.Dispose();
        base.Dispose();
    }
}