using System.Reflection;
using Engine.Logging;

namespace Engine.Decorators;

using Data.RegistryManagers;

/// <summary>
/// Обработчик декорирующих аттрибутов
/// </summary>
public static class RuntimeDecorator
{
    public static void SaveClassDecorators(Type type)
    {
        var methods = type.GetMethods(
            BindingFlags.Instance | BindingFlags.Static | BindingFlags.Public | BindingFlags.NonPublic
        );

        foreach (var method in methods)
        {
            var decorators = method.GetCustomAttributes<QuantumDecoratorAttribute>();
            var quantumDecorators = decorators as QuantumDecoratorAttribute[] ?? decorators.ToArray();

            if (quantumDecorators.Length == 0) continue;
            
            var decoratedMethod = new QuantumMethodInfo
            {
                Method = method,
                Attributes = quantumDecorators
            };

            Registries.DecoratedMethodsRegistry.Register(decoratedMethod);
        }
    }

    public static T? InvokeDecoratedMethod<T>(object? target, MethodInfo method, object?[]? args = null) where T : struct
    {
        var decoratedMethod = Registries.DecoratedMethodsRegistry.Get(method);
        Logger.Debug($"{decoratedMethod}");
        if (decoratedMethod == null) return (T?) method.Invoke(target, args);
        return null;
    }
}