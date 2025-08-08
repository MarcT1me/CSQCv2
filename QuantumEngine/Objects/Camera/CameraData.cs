using System.Diagnostics.CodeAnalysis;
using Engine.Data.Collections;
using OpenTK.Mathematics;

namespace Engine.Objects.Camera;

using Data;
using Actor;

public class CameraData : ActorData
{
    public readonly CameraType CameraType;

    public Vector2 ClipPlanes
    {
        get => Transform.Size.Xy;
        set => Transform.Size.Xy = value;
    }

    public float Fov
    {
        get => Transform.Size.Z;
        set => Transform.Size.Z = value;
    }

    public readonly ConcurrentSet<Identifier> PostProcess = new();

    public float Yaw = 0;
    public float Pitch = 0;

    [method: SetsRequiredMembers]
    public CameraData(
        CameraType cameraType = CameraType.Perspective,
        Vector3? position = null,
        Vector2? clipPlanes = null,
        float fov = 60,
        // MetaData
        string? identifier = null
    ) : base(identifier: identifier)
    {
        CameraType = cameraType;
        Transform.Position = position ?? Vector3.One;
        ClipPlanes = clipPlanes ?? new Vector2(0.001f, 100.0f);
        Fov = fov;
    }
}