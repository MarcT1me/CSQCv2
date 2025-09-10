using MirageAPI;
using MirageAPI.DirectX;
using OpenTK.Mathematics;

namespace QuantumCore.Graphic.Window;

public sealed class GlData
{
    public uint MaxFramesInFlight = 2;

    public uint NumberOfSamples = 1;
    public uint SwapQuality = 0;

    public DX12SwapEffect SwapEffect = DX12SwapEffect.FlipDiscard;
    public DX12ResourceFormat Format = DX12ResourceFormat.RGBA8_UNORM;

    public Rect Viewport = new(0, 0, -1, -1);
    public Vector2 ClipPlanes = new(0.01f, 100.0f);
    public Color4 ClearColor = new(0.08f, 0.16f, 0.18f, 1.0f);
}