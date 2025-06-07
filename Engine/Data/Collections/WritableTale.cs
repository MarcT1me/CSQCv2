#pragma warning disable CS0618 // Type or member is obsolete

namespace Engine.Data.Collections;

using Meta;

/// <summary>
/// Мутабельный объект с данными
/// </summary>
/// <typeparam name="T">Тип данных контейнера</typeparam>
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

    public override void Set(object? key, T? value)
    {
        if (Identifier.GiveFromUncertain(key) is not { } identifier) return;

        if (value == null)
        {
            Data.TryRemove(identifier, out _);
        }
        else
        {
            Data[identifier] = value;
        }
    }
}