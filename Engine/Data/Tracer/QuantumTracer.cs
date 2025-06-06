using System.Reflection;
using System.Collections.Concurrent;

namespace Engine.Data.Tracer;

using Decorators;
using Collections;

/// <summary>
/// Quantum Tracer - ядро системы трассировки метаданных и декораторов
/// </summary>
public static class QuantumTracer
{
    private static readonly ConcurrentDictionary<Type, TypeMetadata> TypeMetadata = new();
    private static readonly ConcurrentDictionary<Type, Type> InterceptedTypes = new();
    private static readonly ConcurrentHashSet<string> ProcessedMethods = new();

    /// <summary>
    /// Регистрирует тип с декорированными методами
    /// </summary>
    public static void RegisterDecoratedType(Type type)
    {
        if (!TypeMetadata.ContainsKey(type))
        {
            var metadata = new TypeMetadata(type);
            TypeMetadata[type] = metadata;
            
            // Автоматически обрабатываем тип для декораторов
            ProcessTypeForDecorators(type);
        }
    }

    /// <summary>
    /// Регистрирует декорированный метод
    /// </summary>
    public static void RegisterDecoratedMethod(Type type, MethodInfo method)
    {
        var key = $"{type.FullName}.{method.Name}";
        if (ProcessedMethods.Add(key))
        {
            if (TypeMetadata.TryGetValue(type, out var typeMetadata))
            {
                typeMetadata.AddDecoratedMethod(method);
            }
        }
    }

    /// <summary>
    /// Обрабатывает тип для применения декораторов
    /// </summary>
    public static void ProcessTypeForDecorators(Type type)
    {
        if (InterceptedTypes.ContainsKey(type))
            return;

        // Создаем перехватывающий тип
        var interceptedType = TypeInterceptor.CreateInterceptedType(type);
        InterceptedTypes[type] = interceptedType;
    }

    /// <summary>
    /// Создает экземпляр с автоматическим применением декораторов
    /// </summary>
    public static T CreateTracedInstance<T>(params object[] args) where T : class
    {
        var originalType = typeof(T);

        // Создаем обычный экземпляр
        var instance = (T)Activator.CreateInstance(originalType, args)!;
        
        // Применяем декораторы через фабрику
        return QuantumDecoratorFactory.CreateIfNeeded(instance);
    }

    /// <summary>
    /// Получает перехватывающий тип для данного типа
    /// </summary>
    public static Type GetInterceptedType(Type originalType)
    {
        return InterceptedTypes.TryGetValue(originalType, out var interceptedType) 
            ? interceptedType 
            : originalType;
    }

    /// <summary>
    /// Проверяет, является ли тип перехваченным
    /// </summary>
    public static bool IsInterceptedType(Type type)
    {
        return InterceptedTypes.ContainsKey(type);
    }

    /// <summary>
    /// Получает метаданные типа
    /// </summary>
    public static TypeMetadata? GetTypeMetadata(Type type)
    {
        return TypeMetadata.TryGetValue(type, out var metadata) ? metadata : null;
    }

    /// <summary>
    /// Получает все зарегистрированные типы с декораторами
    /// </summary>
    public static IEnumerable<Type> GetDecoratedTypes()
    {
        return TypeMetadata.Keys;
    }

    /// <summary>
    /// Получает статистику по декораторам
    /// </summary>
    public static DecoratorStatistics GetStatistics()
    {
        return new DecoratorStatistics
        {
            RegisteredTypes = TypeMetadata.Count,
            InterceptedTypes = InterceptedTypes.Count,
            ProcessedMethods = ProcessedMethods.Count,
            TotalDecoratedMethods = TypeMetadata.Values.Sum(tm => tm.DecoratedMethodCount)
        };
    }
}

/// <summary>
/// Статистика системы декораторов
/// </summary>
public class DecoratorStatistics
{
    public int RegisteredTypes { get; init; }
    public int InterceptedTypes { get; init; }
    public int ProcessedMethods { get; init; }
    public int TotalDecoratedMethods { get; init; }
}