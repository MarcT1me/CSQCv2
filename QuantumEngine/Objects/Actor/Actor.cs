namespace Engine.Objects.Actor;

using Data;
using SceneNode;

public abstract class Actor<TData>(TData nodeData)
    : SceneNode<TData>(nodeData), IPreparableInstance
    where TData : ActorData
{
    public ref Transform Transform
    {
        get
        {
            MetaData.NeedsUpdate = true;
            return ref MetaData.Transform;
        }
    }

    public void PrepareInstance()
    {
    }
}