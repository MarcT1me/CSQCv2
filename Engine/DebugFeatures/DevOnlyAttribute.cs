using System.Reflection;

namespace Engine.DebugFeatures;

using Configuration;
using Decorators;

[AttributeUsage(AttributeTargets.Method)]
public class DevOnlyAttribute(object? defaultValue = null) : Attribute, IDecoratorAttribute
{
    public object? Intercept(object? target, MethodInfo targetMethod, object?[]? args, Func<object?> proceed)
    {
        return BaseConfig.DebugMode ? proceed() : defaultValue;
    }
}