using System.Reflection;

namespace Engine.Data.Meta;

using StringKeyMap = IEnumerable<
    KeyValuePair<
        string,
        object
    >
>;

public class MetaData(Identifier? identifier = null)
{
    public Identifier Identifier { get; } = identifier ?? new Identifier();

    public virtual void Modify(StringKeyMap changes)
    {
        foreach (var (key, value) in changes)
            Modify(key, value);
    }

    public virtual void Modify(string key, object value)
    {
        var property = GetProp(key);

        if (property == null || !property.CanWrite) return;

        // changing type if not correct
        var targetType = property.PropertyType;

        if (value.GetType() != targetType)
        {
            value = Convert.ChangeType(value, targetType);
        }

        // set prop value
        property.SetValue(this, value);
    }

    private PropertyInfo? GetProp(string name) => GetType().GetProperty(
        name, BindingFlags.Public | BindingFlags.Instance | BindingFlags.IgnoreCase
    );
}