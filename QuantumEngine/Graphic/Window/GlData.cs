using MirageAPI.DirectX;
using OpenTK.Mathematics;

namespace Engine.Graphic.Window;

public sealed class GlData
{
    public uint MaxFramesInFlight = 2;
    
    public uint NumberOfSamples = 1;
    public uint SwapQuality = 0;
    
    public DX12SwapEffect SwapEffect = DX12SwapEffect.FlipDiscard;
    public DX12ResourceFormat Format = DX12ResourceFormat.RGBA8_UNORM;
    
    // public uint DepthBits = 24;
    public Vector4 Viewport = new(0.0f, 0.0f, -1.0f, -1.0f);
    public Vector2 ViewportDepth = new(0.01f, 100.0f);
    public Vector4 ClearColor { get; init; } = new(0.08f, 0.16f, 0.18f, 1.0f);
}