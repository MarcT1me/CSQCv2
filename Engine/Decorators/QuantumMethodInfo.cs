using System.Reflection;

namespace Engine.Decorators;

/// <summary>
/// Метод задекорированный аттрибутом
/// </summary>
public record QuantumMethodInfo
{
    public required MethodInfo Method { get; init; }
    public required QuantumAttribute[] Attributes { get; init; }
}