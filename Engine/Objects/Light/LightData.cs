using System.Diagnostics.CodeAnalysis;
using OpenTK.Mathematics;

namespace Engine.Objects.Light;

using Actor;

[method: SetsRequiredMembers]
public class LightData(
    Color4 color,
    float intensity,
    // MetaData
    string? identifier = null)
    : ActorData(identifier: identifier)
{
    public readonly LightType LightType = LightType.Simple;

    public Color4 Color = color;
    public float Intensity = intensity;

    public bool CastShadows { get; set; } = true;
}

[method: SetsRequiredMembers]
public class DirectionalLightData(
    Color4 color,
    float intensity,
    // MetaData
    string? identifier = null
) : LightData(color, intensity, identifier: identifier)
{
    public new readonly LightType LightType = LightType.Directional;
}

[method: SetsRequiredMembers]
public class PointLightData(
    Color4 color,
    float intensity,
    // MetaData
    string? identifier = null
) : LightData(color, intensity, identifier: identifier)
{
    public new readonly LightType LightType = LightType.Point;

    public float Attenuation { get; set; } = 0.1f;
    public float ShadowDistance { get; set; } = 100f;
    public float MaxDistance { get; set; } = 100f;
}

[method: SetsRequiredMembers]
public class SpotLightData(
    Color4 color,
    float intensity,
    // MetaData
    string? identifier = null
) : PointLightData(color, intensity, identifier: identifier)
{
    public new readonly LightType LightType = LightType.Spot;

    public required float InnerAngle { get; set; }
    public required float OuterAngle { get; set; }

    public float FalloffExponent { get; set; } = 1f;
}