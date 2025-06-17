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

    public Vector2i Position;
    public WinFlags Flags;
    public float Opacity;

    public WinData(
        Vector2i size,
        WinFlags flags = 0,
        Vector2i? position = null,
        Vector2? resolutionScaling = null,
        float opacity = 1.0f
    )
    {
        _resolutionScaling = resolutionScaling ?? Vector2.One;
        
        Size = size;
        Flags = flags;
        Position = position ?? new(Window.Centered);
        Opacity = opacity;
    }
}