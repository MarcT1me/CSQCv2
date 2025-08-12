using Engine.Events.QuantumEvents;
using OpenTK.Mathematics;

namespace Engine.Objects.Camera;

using Data;
using Actor;

// using Events.QuantumEvents;

public class Camera(CameraData cameraData)
    : Actor<CameraData>(cameraData),
        IEventful, IUpdatable, IRenderable
{
    public float AspectRatio { get; protected set; }
    public Matrix4 ViewMatrix { get; protected set; }
    public Matrix4 ProjectionMatrix { get; protected set; }

    public bool NeedsUpdate = true;

    public Vector3 Up { get; protected set; } = Transform.UpVector;
    public Vector3 Right { get; protected set; } = Transform.RightVector;
    public Vector3 Forward { get; protected set; } = Transform.ForwardVector;

    public virtual void SetAspectRatio(float width, float height)
    {
        AspectRatio = width / height;
        NeedsUpdate = true;
    }

    public virtual void SetFov(float clipPlane)
    {
        MetaData.Fov = clipPlane;
        NeedsUpdate = true;
    }

    public virtual void SetClipPlane(Vector2 clipPlane)
    {
        MetaData.ClipPlanes = clipPlane;
        NeedsUpdate = true;
    }

    public virtual void HandleEvent(QuantumEvent e)
    {
    }

    public virtual void PreUpdate()
    {
    }

    public virtual void Update()
    {
    }

    public virtual void PostUpdate()
    {
        if (!NeedsUpdate) return;
        MetaData.Pitch = MathHelper.Clamp(MetaData.Pitch, -89.9f, 89.9f);
        UpdateCameraVectors();
        UpdateCameraMatrix();
    }

    public virtual void UpdateCameraVectors()
    {
        float yaw = MathHelper.DegreesToRadians(MetaData.Yaw);
        float pitch = MathHelper.DegreesToRadians(MetaData.Pitch);

        Vector3 newForward = new Vector3
        {
            X = MathF.Cos(yaw) * MathF.Cos(pitch),
            Y = MathF.Sin(pitch),
            Z = MathF.Sin(yaw) * MathF.Cos(pitch)
        };

        Forward = MetaData.Transform.Rotation = Vector3.Normalize(newForward);
        Right = Vector3.Normalize(Vector3.Cross(Forward, Transform.UpVector));
        Up = Vector3.Normalize(Vector3.Cross(Right, Forward));
    }

    public virtual void UpdateCameraMatrix()
    {
        ViewMatrix = Matrix4.LookAt(
            MetaData.Transform.Position,
            MetaData.Transform.Position + Forward,
            Up
        );

        ProjectionMatrix = Matrix4.CreatePerspectiveFieldOfView(
            MathHelper.DegreesToRadians(MetaData.Fov),
            AspectRatio,
            MetaData.ClipPlanes.X,
            MetaData.ClipPlanes.Y
        );
    }

    public virtual void PreRender()
    {
    }

    public virtual void Render()
    {
    }

    public virtual void PostRender()
    {
    }
}