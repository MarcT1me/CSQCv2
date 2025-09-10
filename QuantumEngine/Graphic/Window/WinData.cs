using MirageAPI;
using OpenTK.Mathematics;

namespace QuantumCore.Graphic.Window;

public sealed class WinData(
    Vector2i size,
    Vector2i? position = null,
    float resolutionScaling = 1f,
    WindowStyleInfo? windowStyle = null,
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

    public float AspectRatio => (float)Size.X / Size.Y;

    public Vector2i Position { get; internal set; } =
        position ?? new Vector2i(MirageAPI.Window.USE_DEFAULT_POSITION);

    public WindowStyleInfo WindowStyle { get; } =
        windowStyle ?? new WindowStyleInfo(
            false, false, 255,
            SystemBackdropType.Auto,
            null, null,
            null, null
        );

    public uint VSyncInterval { get; internal set; } = vsyncInterval;
    public bool Fullscreen { get; internal set; } = isFullscreen;
}