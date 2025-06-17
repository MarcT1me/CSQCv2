using OpenTK.Mathematics;

namespace Engine.Objects.Light;

public struct LightUniforms
{
    public LightType Type;
    public Vector3 Position;
    public Vector3 Rotation;
    public Vector3 Size;
    public int Color;
    public float Intensity;
    public bool CastShadows;
    // Point
    public float Attenuation;
    public float ShadowDistance;
    public float MaxDistance;
    // Spot
    public float InnerAngle;
    public float OuterAngle;
    public float FalloffExponent;
}