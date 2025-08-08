namespace Engine.Objects.Scene;

using SceneNode;

public class HeadlessScene<T>
    : SceneNode<T>, IUpdatable
    where T : SceneNodeData
{
    public HeadlessScene(T nodeData) : base(nodeData)
    {
        MetaData.SceneId = Id;
    }
    
    #region Cycle methods

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

    #endregion
}