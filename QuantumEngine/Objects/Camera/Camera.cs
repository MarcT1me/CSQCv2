using OpenTK.Mathematics;
using QuantumCore.Events.QuantumEvents.Window;
using QuantumCore.Time;

namespace QuantumCore.Objects.Camera;

using Actor;
using Events.QuantumEvents;
using Graphic.Window;

public class Camera<TData>(TData cameraData)
    : Actor<TData>(cameraData), IEventful, IUpdatable, IWindowRenderable
    where TData : CameraData
{
    public Matrix4 ViewMatrix { get; protected set; }
    public Matrix4 ProjectionMatrix { get; protected set; }

    public virtual void HandleEvent(QuantumEvent e)
    {
        if (e is WinResizeEvent) MetaData.NeedsUpdate = true;
    }

    public virtual void PreUpdate(ClockMeta clockMeta)
    {
    }

    public virtual void Update(ClockMeta clockMeta)
    {
    }

    public virtual void PostUpdate(ClockMeta clockMeta)
    {
    }

    public virtual void PreRender(WindowData winMeta)
    {
        // check necessity
        if (!MetaData.IsNeedsUpdate) return;

        UpdateCameraMatrix(winMeta);

        MetaData.NeedsUpdate = false;
    }

    public virtual void UpdateCameraMatrix(WindowData winMeta)
    {
        ViewMatrix = Matrix4.LookAt(
            MetaData.Transform.Position,
            MetaData.Transform.Position + MetaData.Transform.Forward,
            MetaData.Transform.Up
        );

        ProjectionMatrix = Matrix4.CreatePerspectiveFieldOfView(
            MathHelper.DegreesToRadians(MetaData.FieldOfView),
            winMeta.WinData.AspectRatio,
            winMeta.GlData.ClipPlanes.X,
            winMeta.GlData.ClipPlanes.Y
        );
    }

    public virtual void Render(WindowData winMeta)
    {
    }

    public virtual void PostRender(WindowData winMeta)
    {
    }
}