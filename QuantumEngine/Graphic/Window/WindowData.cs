using MirageAPI;
using OpenTK.Mathematics;

namespace Engine.Graphic.Window;

using Data;
using Data.Meta;
using Objects;

public class WindowData(
    WinData winData,
    GlData glData,
    // MetaData
    string? identifier
) : MetaData(Identifier.FromUncertain(identifier))
{
    public WinData WinData { get; } = winData;
    public GlData GlData { get; } = glData;
    public ObjectStatusFlags Status => ObjectStatusFlags.All;

    public float AspectRatio => (float)WinData.Size.X / WinData.Size.Y;
    public Vector2i Size => WinData.Size;
    public Vector2i Resolution => WinData.Resolution;
    public SimpleRect Viewport => GlData.Viewport;
    public Vector2 ClipPlanes => GlData.ClipPlanes;
}