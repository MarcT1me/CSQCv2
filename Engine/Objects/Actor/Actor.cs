namespace Engine.Objects.Actor;

using SceneNode;

public abstract class Actor<T>(T nodeData) 
    : SceneNode<T>(nodeData), IPreparableInstance
    where T : ActorData
{
    public static void PrepareInstance()
    {
    }
}