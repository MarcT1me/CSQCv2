using System.Reflection;

namespace QuantumCore.Decorators;

/// <summary>
/// Метод задекорированный аттрибутом
/// </summary>
public sealed record QuantumMethodInfo(
    MethodInfo Method
)
{
    public bool Equals(QuantumMethodInfo? other)
    {
        return other != null && Method.Equals(other.Method);
    }

    public override int GetHashCode()
    {
        return HashCode.Combine(Method);
    }
}