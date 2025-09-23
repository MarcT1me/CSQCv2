using MirageAPI.DirectX.Resource;
using OpenTK.Mathematics;

namespace QuantumCore.Graphic.Font;

public struct FontCharacter
{
    public DX12Texture Texture { get; init; }
    public Vector2i Size { get; init; }
    public Vector2i Bearing { get; init; }
    public int Advance { get; init; }
}