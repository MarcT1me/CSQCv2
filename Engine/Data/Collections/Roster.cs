#pragma warning disable CS0618 // Type or member is obsolete

using System.Collections.Concurrent;

namespace Engine.Data.Collections;

using Meta;

public class Roster<T> : DataContainer<T>
{
    public ConcurrentDictionary<Identifier, Roster<T>> Branches { get; } = [];

    public Roster(MetaData metaData)
        : base(metaData)
    {
    }

    public Roster(MetaData metaData, IEnumerable<KeyValuePair<Identifier, T>> data)
        : base(metaData, data)
    {
    }

    #region Branch creating operations

    public Roster<T> NewBranch(MetaData metaData) => NewBranch(metaData, new List<KeyValuePair<Identifier, T>>());

    public Roster<T> NewBranch(MetaData metaData, IEnumerable<KeyValuePair<Identifier, T>> data)
    {
        var branch = new Roster<T>(metaData, data);
        SaveBranch(branch);
        return branch;
    }

    public void SaveBranch(Roster<T> branch)
    {
        Branches[branch.Id] = branch;
    }

    public Roster<T>? GetBranch(object id)
    {
        Identifier? identifier = Identifier.GiveFromUncertain(id);
        return identifier == null ? null : Branches.GetValueOrDefault(identifier);
    }

    #endregion

    public override void Set(Identifier key, T? value)
    {
        if (value == null)
        {
            Data.TryRemove(key, out _);
        }
        else
        {
            Data[key] = value;
        }
    }
}