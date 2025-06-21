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
    public float Opacity;

    public Vector2i MinSize;
    public Vector2i MaxSize;

    public WinData(
        Vector2i size,
        Vector2i? position = null,
        Vector2? resolutionScaling = null,
        float opacity = 1.0f,
        Vector2i? minSize = null,
        Vector2i? maxSize = null
    )
    {
        _resolutionScaling = resolutionScaling ?? Vector2.One;

        Size = size;
        Opacity = opacity;
        Position = position ?? new(100);
        MinSize = minSize ?? Vector2i.Zero;
        MaxSize = maxSize ?? size;
    }
}