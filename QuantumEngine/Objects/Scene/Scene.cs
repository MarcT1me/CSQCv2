namespace Engine.Objects.Scene;

using Events.QuantumEvents;
using SceneNode;

public class Scene(SceneNodeData nodeData) 
    : BaseScene(nodeData),
        IEventful, IUpdatable, IRenderable
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

    public virtual void PreUpdate()
    {
        foreach (var updatableChild in IterChildren<IUpdatable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsDynamic())
                updatableChild.PreUpdate();
        }
    }

    public virtual void Update()
    {
        foreach (var updatableChild in IterChildren<IUpdatable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsDynamic())
                updatableChild.Update();
        }
    }

    public virtual void PostUpdate()
    {
        foreach (var updatableChild in IterChildren<IUpdatable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsDynamic())
                updatableChild.PostUpdate();
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