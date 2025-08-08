namespace Engine.Objects.Scene;

using Events.QuantumEvents;
using SceneNode;

public class Scene<T>(T nodeData)
    : HeadlessScene<T>(nodeData), IEventful, IRenderable
    where T : SceneNodeData
{
    #region Cycle methods

    public virtual void HandleEvent(QuantumEvent e)
    {
        foreach (var eventfulChild in IterChildren<IEventful>())
        {
            if (eventfulChild.IsActive())
                eventfulChild.HandleEvent(e);
        }
    }

    public virtual void PreRender()
    {
        foreach (var updatableChild in IterChildren<IRenderable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsVisible())
                updatableChild.PreRender();
        }
    }

    public virtual void Render()
    {
        foreach (var updatableChild in IterChildren<IRenderable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsVisible())
                updatableChild.Render();
        }
    }

    public virtual void PostRender()
    {
        foreach (var updatableChild in IterChildren<IRenderable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsVisible())
                updatableChild.PostRender();
        }
    }

    #endregion
}