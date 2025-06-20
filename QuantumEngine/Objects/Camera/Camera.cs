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

    public Vector3 Up { get; protected set; } = Transform.NewUpVector();
    public Vector3 Right { get; protected set; } = Transform.NewRightVector();
    public Vector3 Forward { get; protected set; } = Transform.NewForwardVector();

    public void SetAspectRatio(float width, float height)
    {
        AspectRatio = width / height;
        NeedsUpdate = true;
    }

    public virtual void HandleEvent(QuantumEvent e)
    {
    }

    public virtual void PreUpdate()
    {
        // не требуется
    }

    public virtual void Update()
    {
        // не требуется
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

        Forward = Vector3.Normalize(newForward);
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

    public void PreRender()
    {
    }

    public void Render()
    {
    }

    public void PostRender()
    {
    }
}