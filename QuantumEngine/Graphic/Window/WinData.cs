using OpenTK.Mathematics;

namespace Engine.Graphic.Window;

public sealed class WinData(
    Vector2i size,
    Vector2i? position = null,
    float resolutionScaling = 1f,
    float opacity = 1f,
    uint vsyncInterval = 0,
    bool isFullscreen = false
)
{
    public Vector2i Size { get; internal set; } = size;
    public float ResolutionScaling { get; internal set; } = resolutionScaling;

    public Vector2i Resolution => new(
        (int)(ResolutionScaling * Size.X),
        (int)(ResolutionScaling * Size.Y)
    );

    public Vector2i Position { get; internal set; } = position ?? new Vector2i(Window.UseDefault);
    public float Opacity { get; internal set; } = opacity;
    public uint VSyncInterval { get; internal set; } = vsyncInterval;
    public bool Fullscreen { get; internal set; } = isFullscreen;
}