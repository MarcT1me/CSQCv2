#pragma warning disable CS0618 // Type or member is obsolete

namespace Engine.Data.Arrays;

using Meta;

public sealed class WritableTale<T> : DataContainer<T>
{
    public WritableTale(MetaData metaData)
        : base(metaData)
    {
    }

    public WritableTale(MetaData metaData, IEnumerable<KeyValuePair<Identifier, T>> data)
        : base(metaData, data)
    {
    }

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