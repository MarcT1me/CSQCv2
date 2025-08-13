namespace Engine.Objects.Actor;

using Data;
using SceneNode;

public class ActorData(
    Transform? transform = null,
    int? priority = null,
    Identifier? modelId = null,
    // MetaData
    string? identifier = null
) : SceneNodeData(identifier: identifier)
{
    public Transform Transform = transform ?? new Transform();
    public int Priority = priority ?? 0;
    public Identifier? ModelId = modelId;
}