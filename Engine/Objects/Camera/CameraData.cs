using System.Collections.Concurrent;
using System.Diagnostics.CodeAnalysis;
using Engine.Data.Collections;
using OpenTK.Mathematics;

namespace Engine.Objects.Camera;

using Data;
using Actor;

[method: SetsRequiredMembers]
public class CameraData(
    CameraType cameraType,
    Vector2 clipPlanes,
    int fov = 60,
    // MetaData
    string? identifier = null
) : ActorData(identifier: identifier)
{
    public readonly CameraType CameraType = cameraType;
    public Vector2 ClipPlanes = clipPlanes;
    public int Fov = fov;
    public readonly ConcurrentSet<Identifier> PostProcess = new();

    public float Yaw = 0;
    public float Pitch = 0;

    public void AddPostProcess(Identifier identifier)
    {
        PostProcess.Add(identifier);
    }

    public void RemovePostProcess(Identifier identifier)
    {
        PostProcess.Remove(identifier);
    }
}