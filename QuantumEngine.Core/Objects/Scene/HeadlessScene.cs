using QuantumCore.Time;

namespace QuantumCore.Objects.Scene;

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

    public virtual void PreUpdate(ClockMeta clockMeta)
    {
        foreach (var updatableChild in IterChildren<IUpdatable>())
        {
            if (updatableChild.IsDynamic)
                updatableChild.PreUpdate(clockMeta);
        }
    }

    public virtual void Update(ClockMeta clockMeta)
    {
        foreach (var updatableChild in IterChildren<IUpdatable>())
        {
            if (updatableChild.IsDynamic)
                updatableChild.Update(clockMeta);
        }
    }

    public virtual void PostUpdate(ClockMeta clockMeta)
    {
        foreach (var updatableChild in IterChildren<IUpdatable>())
        {
            if (updatableChild.IsDynamic)
                updatableChild.PostUpdate(clockMeta);
        }
    }

    #endregion
}