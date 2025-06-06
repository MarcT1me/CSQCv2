using System.Dynamic;

namespace Engine.Data.Meta;

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