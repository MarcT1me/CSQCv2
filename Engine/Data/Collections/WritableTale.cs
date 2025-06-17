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

    public WritableTale(MetaData metaData, Dictionary<Identifier, T> data)
        : base(metaData, data)
    {
    }

    public override void Set(object key, T? value)
    {
        if (Identifier.GiveFromUncertain(key) is not { } identifier) return;

        if (value == null)
        {
            Pop(key);
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