using System.Reflection;

namespace Engine.Decorators;

/// <summary>
/// Метод задекорированный аттрибутом
/// </summary>
public record QuantumMethodInfo(MethodInfo Method, IEnumerable<QuantumDecoratorAttribute> Decorators);