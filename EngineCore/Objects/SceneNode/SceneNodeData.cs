
namespace Engine.Objects.SceneNode;

using Data;
using Data.Meta;
using Data.Collections;
using Extensions;

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

    public bool NeedsUpdate
    {
        get => Status.HasFlag(ObjectStatusFlags.NeedsUpdate);
        set => Status.ChangeFlag(ObjectStatusFlags.NeedsUpdate, value);
    }
}