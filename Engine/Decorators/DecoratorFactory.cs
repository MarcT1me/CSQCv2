using System.Reflection;
using System.Runtime.CompilerServices;

namespace Engine.Decorators;

public static class DecoratorFactory
{
    private static readonly ConditionalWeakTable<object, object> ProxyCache = new();
    
    /// <summary>
    /// Создает прокси для объекта, если у него есть декорированные методы
    /// </summary>
    public static T CreateIfNeeded<T>(T instance) where T : class
    {
        // Проверяем кэш
        if (ProxyCache.TryGetValue(instance, out var cachedProxy))
            return (T)cachedProxy;
        
        // Проверяем, есть ли декорированные методы
        if (!HasDecoratedMethods<T>())
            return instance;
        
        // Создаем прокси
        var proxy = DecoratorDispatchProxy<T>.Create(instance);
        if (proxy is not null)
        {
            ProxyCache.Add(instance, proxy);
            return proxy;
        }
        
        return instance;
    }
    
    private static bool HasDecoratedMethods<T>()
    {
        return typeof(T).GetMethods(BindingFlags.Public | BindingFlags.Instance)
            .Any(m => m.GetCustomAttributes().OfType<IDecoratorAttribute>().Any());
    }
}