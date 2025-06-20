namespace Engine.Objects.SceneNode;

using Data;
using Data.Meta;
using Data.Collections;

public class SceneNodeData(
    ObjectStatusFlags? status = null,
    // MetaData
    string? identifier = null
) : MetaData(identifier)
{
    public Identifier? SceneId;
    public Identifier? ParentId;
    public ConcurrentSet<Identifier> ChildrenIds = new();
    public ObjectStatusFlags Status = status ?? ObjectStatusFlags.All;
}