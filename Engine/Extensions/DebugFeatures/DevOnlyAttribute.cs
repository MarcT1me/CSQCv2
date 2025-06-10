using System.Reflection;

namespace Engine.Extensions.DebugFeatures;

using Configuration;
using Decorators;

/// <summary>
/// Декорирующий аттрибут, исключающий выполнения в Release сборке 
/// </summary>
/// <param name="defaultValue">Значение по умолчанию, если метод где-то используется</param>
[AttributeUsage(AttributeTargets.Method | AttributeTargets.Event | AttributeTargets.Constructor)]
public class DevOnlyAttribute(object? defaultValue = null) : QuantumDecoratorAttribute
{
    public override object? Intercept(object? target, MethodInfo targetMethod, object?[]? args, Func<object?> proceed)
    {
        return BaseConfig.DebugMode ? proceed() : defaultValue;
    }
}