using System.Dynamic;

namespace Engine.Data;

/// <summary>
/// Динамический объект для передачи данных
/// </summary>
/// <remarks>Чаще всего используется в движке. Убедитесь в присутствии аттрибута в объекте перед его изъятием</remarks>
public class QExtended : DynamicObject
{
    protected readonly Dictionary<string, object> Properties = new();

    public override bool TryGetMember(GetMemberBinder binder, out object? result)
    {
        if (Properties.TryGetValue(binder.Name, out result))
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
            Properties[binder.Name] = value;
            return true;
        }

        return false;
    }
}