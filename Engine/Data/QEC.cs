using System.Dynamic;

namespace Engine.Data;

public class QExtended : DynamicObject
{
    private readonly Dictionary<string, object> _properties = new();

    public override bool TryGetMember(GetMemberBinder binder, out object? result)
    {
        if (_properties.TryGetValue(binder.Name, out result))
        {
            return true;
        }

        result = null;
        return false;
    }

    public override bool TrySetMember(SetMemberBinder binder, object? value)
    {
        if (value != null)
        {
            _properties[binder.Name] = value;
            return true;
        }

        return false;
    }
}