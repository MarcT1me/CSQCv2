using System.Runtime.InteropServices;
using MirageAPI.DirectX;
using MirageAPI.DirectX.Command;
using MirageAPI.DirectX.Descriptors;
using MirageAPI.DirectX.Pipeline;
using MirageAPI.DirectX.Resource;
using MirageAPI.DirectX.Shader;
using OpenTK.Mathematics;
using QuantumCore.Asset;
using QuantumCore.Asset.Shader;
using QuantumCore.Logging;
using QuantumCore.Objects;
using SharpFont;

namespace QuantumCore.Graphic.Font;

using Data.Meta;

[StructLayout(LayoutKind.Sequential)]
struct FontVertex
{
    public Vector3 Position;
    public Vector2 UV;
}

[StructLayout(LayoutKind.Sequential)]
public struct FontRenderData
{
    public Matrix4 Transform;
    public Color4 Color;
}

public class Font : MetaObject<FontData>, IPreparable, IDisposable
{
    private readonly Face _face;
    private readonly Library _fontLib = new();
    private readonly Dictionary<char, FontCharacter> _characters = new();

    // render natives
    private readonly DX12Fence _fence;
    private readonly DX12CommandQueue _commandQueue;
    private readonly DX12CommandList _commandList;
    private readonly DX12ShaderProgram _shaderProgram;
    private readonly DX12Buffer _vertexBuffer;

    public Font(
        string name, uint size, string family,
        FontStyle style = FontStyle.Normal,
        string fileDir = "C:\\Windows\\Fonts",
        bool autoload = true
    ) : base(new FontData(name, size, family, style, fileDir))
    {
        if (!File.Exists(MetaData.FontFile))
            throw new FileNotFoundException("Font file not found: " + MetaData.FontFile);

        var fontData = AssetLoader.ReadBinary(MetaData.FontFile);
        _face = new Face(_fontLib, fontData, 0);
        _face.SetPixelSizes(0, size);
        MetaData.Height = (uint)_face.Size.Metrics.Height.Value >> 6;

        _fence = new DX12Fence(null, 1);
        _commandQueue = new DX12CommandQueue("font renderer", DX12CommandListType.Direct, _fence);
        _commandList = new DX12CommandList("font renderer", _commandQueue);

        _shaderProgram = new DX12ShaderProgram(
            "Font Renderer shader program",
            [
                ((ShaderData)AssetManager.Load(
                    new(
                        "vertexShader", "QuantumCore:Shaders/font.hlsl",
                        identifier: "FontShader-Vert"
                    )
                ).Content).GetNativeShader,
                ((ShaderData)AssetManager.Load(
                    new(
                        "pixelShader", "QuantumCore:Shaders/font.hlsl",
                        identifier: "FontShader-Pix"
                    )
                ).Content).GetNativeShader
            ],
            [
                new DX12InputElement("POSITION", DX12ResourceFormat.RGB32_FLOAT, 0),
                new DX12InputElement("UV", DX12ResourceFormat.RG32_FLOAT, 32 * 3)
            ],
            []
        );

        FontVertex[] vertices =
        [
            new()
            {
                Position = new Vector3(-1.0f, 0.0f, 1.0f),
                UV = new Vector2(0, 0)
            },
            new()
            {
                Position = new Vector3(1.0f, 0.0f, 1.0f),
                UV = new Vector2(1, 0)
            },
            new()
            {
                Position = new Vector3(-1.0f, 0.0f, -1.0f),
                UV = new Vector2(0, 1)
            },

            new()
            {
                Position = new Vector3(1.0f, 0.0f, 1.0f),
                UV = new Vector2(1, 0)
            },
            new()
            {
                Position = new Vector3(1.0f, 0.0f, -1.0f),
                UV = new Vector2(1, 1)
            },
            new()
            {
                Position = new Vector3(-1.0f, 0.0f, -1.0f),
                UV = new Vector2(0, 1)
            }
        ];

        try
        {
            int vertexSize = Marshal.SizeOf<FontVertex>();

            _vertexBuffer = new DX12Buffer(
                DX12ResourceConfig.VertexBufferConfig(
                    "FontRenderer vertex buffer",
                    6,
                    (uint)vertexSize,
                    DX12ResourceFlags.None
                )
            );

            unsafe
            {
                fixed (FontVertex* verticesPtr = vertices)
                {
                    int bufferSize = 6 * vertexSize;
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

        Preload(0, 128);
        _shaderProgram.Prepare();
        _shaderProgram.DescriptorHeap.MetaData.AutoAllocate = autoload;
    }

    public void Preload(uint start, uint end)
    {
        for (char c = (char)start; c <= end; c++)
        {
            if (LoadChar(c) is null) break;
        }
    }

    public void Preload(IEnumerable<char> collection)
    {
        foreach (char c in collection)
        {
            if (LoadChar(c) is null) break;
        }
    }

    public FontCharacter? LoadChar(char c)
    {
        if (_characters.TryGetValue(c, out var value))
        {
            return value;
        }

        try
        {
            _face.LoadChar(c, LoadFlags.Render, LoadTarget.Normal);
            var glyph = _face.Glyph;
            var bitmap = glyph.Bitmap;

            var texture = new DX12Texture(
                DX12ResourceConfig.TextureConfig(
                    "",
                    DX12ShaderVisibility.All,
                    (uint)bitmap.Width, (uint)bitmap.Rows,
                    DX12ResourceFormat.R8_UNORM, DX12ResourceFlags.None,
                    0, 1,
                    DX12TextureType.Texture2D,
                    DX12ViewDimension.Texture2D
                )
            );
            texture.UploadData(bitmap.BufferData);
            _shaderProgram.BindResource(texture);

            return _characters[c] = new FontCharacter
            {
                Texture = texture,
                Size = new Vector2i(bitmap.Width, bitmap.Rows),
                Bearing = new Vector2i(glyph.BitmapLeft, glyph.BitmapTop),
                Advance = glyph.Advance.X.Value
            };
        }
        catch (Exception ex)
        {
            Logger.Warning($"Failed to load character {c}. Any error occured: {ex.Message}");
            return null;
        }
    }

    public void Prepare()
    {
        _shaderProgram.Prepare();
    }

    public void Render(DX12Descriptor target, string text, Color4 color)
    {
        // pre
        _commandQueue.Signal();
        _commandQueue.Wait();

        // render
        _shaderProgram.BeginFrame(target, _commandList);

        foreach (var _ in text)
        {
            // _shaderProgram.Bind(_characters[character].Texture); // TODO: binding textures
            _shaderProgram.Render(DX12PrimitiveTopology.TriangleList, _vertexBuffer, null, 6);
        }

        _shaderProgram.ReleaseFrame();

        // post
        _commandQueue.ExecuteList(_commandList);
        _commandQueue.WaitForCompletion();
    }

    public FontCharacter GetChar(char c)
    {
        if (!_characters.TryGetValue(c, out var character))
        {
            character = LoadChar(c) ?? _characters['\0'];
        }

        return character;
    }

    public void Dispose()
    {
        UnloadAll();
        _shaderProgram.Dispose();
        _fence.Dispose();
        _characters.Clear();
        _face.Dispose();
    }

    public void UnloadAll()
    {
        foreach (char character in _characters.Keys)
        {
            UnloadCharacter(character);
        }
    }

    public void Unload(uint start, uint end)
    {
        for (char c = (char)start; c <= end; c++)
        {
            UnloadCharacter(c);
        }
    }

    public void Unload(IEnumerable<char> collection)
    {
        foreach (char c in collection)
        {
            UnloadCharacter(c);
        }
    }

    public void UnloadCharacter(char charCode)
    {
        if (_characters.TryGetValue(charCode, out var value))
        {
            value.Texture.Dispose();
            _characters.Remove(charCode);
        }
    }
}