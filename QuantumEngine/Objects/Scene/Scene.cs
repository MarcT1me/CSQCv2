using Engine.Graphic.Window;

namespace Engine.Objects.Scene;

using Events.QuantumEvents;
using SceneNode;

public class Scene<T>(T nodeData)
    : HeadlessScene<T>(nodeData), IEventful, IWindowRenderable
    where T : SceneNodeData
{
    #region Cycle methods

    public virtual void HandleEvent(QuantumEvent e)
    {
        foreach (var eventfulChild in IterChildren<IEventful>())
        {
            if (eventfulChild.IsActive)
                eventfulChild.HandleEvent(e);
        }
    }

    public virtual void PreRender(WindowData winMeta)
    {
        foreach (var updatableChild in IterChildren<IWindowRenderable>())
        {
            if (updatableChild.IsVisible)
                updatableChild.PreRender(winMeta);
        }
    }

    public virtual void Render(WindowData winMeta)
    {
        foreach (var updatableChild in IterChildren<IWindowRenderable>())
        {
            if (updatableChild.IsVisible)
                updatableChild.Render(winMeta);
        }
    }

    public virtual void PostRender(WindowData winMeta)
    {
        foreach (var updatableChild in IterChildren<IWindowRenderable>())
        {
            if (updatableChild.IsVisible)
                updatableChild.PostRender(winMeta);
        }
    }

    #endregion
}