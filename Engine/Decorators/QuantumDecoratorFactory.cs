namespace Engine.Decorators;

using System.Reflection;
using System.Runtime.CompilerServices;
using Data.Tracer;

public static class QuantumDecoratorFactory
{
    private static readonly ConditionalWeakTable<object, object> DecoratorCache = new();

    /// <summary>
    /// Создает декорированный объект если необходимо
    /// </summary>
    public static T CreateIfNeeded<T>(T instance) where T : class
    {
        // Проверяем кэш
        if (DecoratorCache.TryGetValue(instance, out var cachedDecorated))
            return (T)cachedDecorated;

        var type = typeof(T);

        // Если нет декорированных методов, возвращаем оригинал
        if (!HasDecoratedMethods(type))
            return instance;

        // Регистрируем в QuantumTracer
        QuantumTracer.RegisterDecoratedType(type);

        // Получаем перехватывающий тип
        var interceptedType = TypeInterceptor.CreateInterceptedType(type);

        if (interceptedType == type)
        {
            // Если перехватывающий тип не создан, используем прокси
            return CreateQuantumProxy(instance);
        }

        // Создаем экземпляр перехватывающего типа
        try
        {
            var decoratedInstance = (T)Activator.CreateInstance(interceptedType)!;

            // Копируем состояние из оригинального объекта
            CopyObjectState(instance, decoratedInstance);

            DecoratorCache.Add(instance, decoratedInstance);

            // Уведомляем QuantumTracer
            var metadata = QuantumTracer.GetTypeMetadata(type);
            metadata?.SetCustomData("decorator_applied_at", DateTime.UtcNow);

            return decoratedInstance;
        }
        catch
        {
            // Если не удалось создать перехватывающий тип, используем прокси
            return CreateQuantumProxy(instance);
        }
    }

    /// <summary>
    /// Создает прокси с интеграцией в QuantumTracer (fallback метод)
    /// </summary>
    private static T CreateQuantumProxy<T>(T instance) where T : class
    {
        var proxy = DecoratorDispatchProxy<T>.Create(instance);
        if (proxy != null)
        {
            DecoratorCache.Add(instance, proxy);

            // Уведомляем QuantumTracer о создании прокси
            var metadata = QuantumTracer.GetTypeMetadata(typeof(T));
            metadata?.SetCustomData("proxy_created_at", DateTime.UtcNow);

            return proxy;
        }

        return instance;
    }

    /// <summary>
    /// Копирует состояние объекта (поля и свойства)
    /// </summary>
    private static void CopyObjectState<T>(T source, T target) where T : class
    {
        var type = typeof(T);

        // Копируем поля
        var fields = type.GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
        foreach (var field in fields)
        {
            if (!field.IsInitOnly) // Пропускаем readonly поля
            {
                var value = field.GetValue(source);
                field.SetValue(target, value);
            }
        }

        // Копируем свойства
        var properties = type.GetProperties(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
        foreach (var property in properties)
        {
            if (property is { CanRead: true, CanWrite: true } && property.GetSetMethod() != null)
            {
                var value = property.GetValue(source);
                property.SetValue(target, value);
            }
        }
    }

    private static bool HasDecoratedMethods(Type type)
    {
        return type.GetMethods(BindingFlags.Public | BindingFlags.Instance)
            .Any(m => m.GetCustomAttributes().OfType<IDecoratorAttribute>().Any());
    }
}