#pragma warning disable CS0618 // Type or member is obsolete

namespace QuantumCore.Data.Collections;

using Meta;

/// <summary>
/// Мутабельный объект с данными
/// </summary>
/// <typeparam name="T">Тип данных контейнера</typeparam>
public sealed class WritableTale<T> : DataContainer<T>
{
    public WritableTale(MetaData metaData, object? lockObject = null)
        : base(metaData, lockObject)
    {
    }

    public WritableTale(MetaData metaData, Dictionary<Identifier, T> data, object? lockObject = null)
        : base(metaData, data, lockObject)
    {
    }

    public override void Set(object key, T? value)
    {
        if (Identifier.GiveFromUncertain(key) is not { } identifier) return;

        if (Lock is null)
        {
            _SetOperation(identifier, value);
        }
        else
        {
            lock (Lock)
            {
                _SetOperation(identifier, value);
            }
        }
    }

    private void _SetOperation(Identifier identifier, T? value)
    {
        if (value == null)
        {
            Pop(identifier);
        }
        else
        {
            Data[identifier] = value;
        }
    }

    public override T? Pop(object key)
    {
        var identifier = Identifier.GiveFromUncertain(key);

        if (identifier == null) return default;

        Data.TryRemove(identifier, out var obj);
        return (T)obj!;
    }
}