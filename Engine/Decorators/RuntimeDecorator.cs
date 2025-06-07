using System.Reflection;

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
            var decorators = method.GetCustomAttributes().OfType<IQuantumDecorator>();
            var quantumDecorators = decorators as IQuantumDecorator[] ?? decorators.ToArray();

            if (quantumDecorators.Length == 0) continue;

            var decoratedMethod = new DecoratedMethodInfo
            {
                Method = method,
                Decorators = quantumDecorators
            };

            Registries.DecoratedMethodsRegistry.Register(decoratedMethod);
        }
    }

    public static object? InvokeDecoratedMethod(object? target, MethodInfo method, object?[]? args = null)
    {
        var decoratedMethod = Registries.DecoratedMethodsRegistry.Get(method);

        if (decoratedMethod == null) return method.Invoke(target, args);

        Func<object?>? proceed = null;

        // Строим цепочку вызовов в обратном порядке
        foreach (var decorator in decoratedMethod.Decorators.Reverse())
        {
            var currentProceed = proceed;
            var currentDecorator = decorator;

            proceed = () => currentDecorator.Intercept(
                target,
                method,
                args,
                currentProceed ?? (() => method.Invoke(target, args))
            );
        }

        return proceed?.Invoke();
    }
}