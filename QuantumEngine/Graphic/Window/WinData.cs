using OpenTK.Mathematics;

namespace Engine.Graphic.Window;

public sealed class WinData
{
    private Vector2i _size;
    private Vector2 _resolutionScaling;
    public Vector2i Resolution { get; private set; }

    public Vector2 ResolutionScaling
    {
        get => _resolutionScaling;
        set
        {
            _resolutionScaling = value;
            Resolution = (Vector2i)(Size * value);
        }
    }

    public Vector2i Size
    {
        get => _size;
        set
        {
            _size = value;
            ResolutionScaling = ResolutionScaling;
        }
    }

    public Vector2i Position { get; internal set; }
    public float Opacity { get; internal set; }
    public uint VSyncInterval { get; internal set; }
    public bool Fullscreen { get; internal set; }

    public WinData(
        Vector2i size,
        Vector2i? position = null,
        Vector2? resolutionScaling = null,
        float opacity = 1.0f,
        uint vsyncInterval = 0,
        bool isFullscreen = true
    )
    {
        _resolutionScaling = resolutionScaling ?? Vector2.One;

        Size = size;
        Opacity = opacity;
        Position = position ?? new Vector2i(Window.UseDefault);
        VSyncInterval = vsyncInterval;
        Fullscreen = isFullscreen;
    }
}