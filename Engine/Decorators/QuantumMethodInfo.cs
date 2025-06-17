using System.Reflection;

namespace Engine.Decorators;

/// <summary>
/// Метод задекорированный аттрибутом
/// </summary>
public sealed record QuantumMethodInfo(
    MethodInfo Method, 
    IEnumerable<QuantumDecoratorAttribute> Decorators)
{
    public bool Equals(QuantumMethodInfo? other)
    {
        return other != null && 
               Method.Equals(other.Method) &&
               Decorators.SequenceEqual(other.Decorators);
    }
    
    public override int GetHashCode()
    {
        return HashCode.Combine(Method, Decorators);
    }
}