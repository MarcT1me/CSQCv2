using OpenTK.Mathematics;
using QuantumCore.Data.Collections;

namespace QuantumCore.Objects.Camera;

using Data;
using Actor;

public class CameraData : ActorData
{
    public readonly CameraType CameraType;
    private float _fov;

    public float FieldOfView
    {
        get => _fov;
        set
        {
            _fov = value;
            NeedsUpdate = true;
        }
    }

    public readonly ConcurrentSet<Identifier> PostProcess = new();

    public CameraData(
        CameraType cameraType = CameraType.Perspective,
        Vector3? position = null,
        Vector3? rotation = null,
        float fov = 70,
        // MetaData
        string? identifier = null
    ) : base(identifier: identifier)
    {
        CameraType = cameraType;
        _fov = fov;
        var rotationVector = rotation ?? Vector3.Zero;
        Transform = new Transform(
            position ?? Vector3.Zero,
            Quaternion.FromEulerAngles(
                MathHelper.DegreesToRadians(rotationVector.X),
                MathHelper.DegreesToRadians(rotationVector.Y),
                MathHelper.DegreesToRadians(rotationVector.Z)
                ),
            Vector3.One
        );
    }
}