using System.Reflection;

namespace Engine.Decorators;

public class DecoratorDispatchProxy<T> : DispatchProxy where T : class
{
    private T? _target;

    protected override object? Invoke(MethodInfo? targetMethod, object?[]? args)
    {
        if (targetMethod is null || _target is null)
            return null;

        var attributes = targetMethod.GetCustomAttributes().OfType<IDecoratorAttribute>().ToArray();

        return attributes.Length == 0
            ? targetMethod.Invoke(_target, args)
            : ExecuteDecoratorChain(attributes, 0, _target, targetMethod, args);
    }

    private object? ExecuteDecoratorChain(
        IDecoratorAttribute[] decorators,
        int index,
        object target,
        MethodInfo method,
        object?[]? args
    )
    {
        if (index >= decorators.Length)
        {
            return method.Invoke(target, args);
        }

        var currentDecorator = decorators[index];

        Func<object?> proceed = () => ExecuteDecoratorChain(decorators, index + 1, target, method, args);

        return currentDecorator.Intercept(target, method, args, proceed);
    }

    public static T? Create(T? target)
    {
        if (target is null) return null;
        
        if (Create<T, DecoratorDispatchProxy<T>>() is not DecoratorDispatchProxy<T> proxy) 
            return null;
        
        proxy._target = target;
        return proxy as T;
    }
}