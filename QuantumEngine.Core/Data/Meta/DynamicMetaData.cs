using System.Dynamic;

namespace QuantumCore.Data.Meta;

/// <summary>
/// Метаданные, динамически меняющиеся в ходе выполнения
/// </summary>
/// <param name="core">Метаданные, которые стоит считать динамическими</param>
/// <remarks>Невозможно превратить обратно, но есть возможность прочитать данные ядра</remarks>
public class DynamicMetaData(MetaData core) : DynamicObject
{
    private readonly Dictionary<string, object?> _data = new();
    private MetaData CoreMetadata { get; } = core;

    public override bool TryGetMember(GetMemberBinder binder, out object? result)
    {
        var property = CoreMetadata.GetType().GetProperty(binder.Name);
        if (property != null)
        {
            result = property.GetValue(CoreMetadata);
            return true;
        }

        if (_data.TryGetValue(binder.Name, out result))
            return true;

        result = null;
        return false;
    }

    public override bool TrySetMember(SetMemberBinder binder, object? value)
    {
        // Пытаемся установить в основные свойства
        var property = CoreMetadata.GetType().GetProperty(binder.Name);
        if (property != null && property.CanWrite)
        {
            property.SetValue(CoreMetadata, value);
            return true;
        }

        _data[binder.Name] = value;
        return true;
    }
}