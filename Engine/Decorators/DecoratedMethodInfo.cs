using System.Reflection;

namespace Engine.Decorators;

/// <summary>
/// Метод задекорированный аттрибутом
/// </summary>
public record DecoratedMethodInfo
{
    public required MethodInfo Method { get; init; }
    public required IQuantumDecorator[] Decorators { get; init; }
}