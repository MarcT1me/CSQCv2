namespace QuantumCore.Objects.Actor;

using SceneNode;

public abstract class Actor<TData>(TData nodeData)
    : SceneNode<TData>(nodeData), IPreparableInstance
    where TData : ActorData
{
    public void PrepareInstance()
    {
    }
}