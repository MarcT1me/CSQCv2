using OpenTK.Mathematics;

namespace Engine.Graphic.OpenGl;

public sealed class GlData
{
    // Static init info
    public static int NumberOfSamples = 8;
    public static int DepthBits = 24;
    public static Vector2i ApiVersions = new(3, 3);
    
    // other
    public Vector4 ClearColor = new Vector4(0.08f, 0.16f, 0.18f, 1.0f);
}