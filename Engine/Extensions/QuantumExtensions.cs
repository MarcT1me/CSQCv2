namespace Engine.Extensions;

using Data.Tracer;
using Data.Meta;
using Decorators;

public static class QuantumExtensions
{
    /// <summary>
    /// Создает объект с автоматическими декораторами через QuantumTracer
    /// </summary>
    public static T CreateQuantum<T>(params object[] args) where T : class
    {
        return QuantumTracer.CreateTracedInstance<T>(args);
    }

    /// <summary>
    /// Применяет декораторы к существующему объекту (если необходимо)
    /// </summary>
    public static T WithQuantumDecorators<T>(this T instance) where T : class
    {
        return QuantumDecoratorFactory.CreateIfNeeded(instance);
    }

    /// <summary>
    /// Создает QuantumMetaObject с автоматическими декораторами
    /// </summary>
    public static QuantumMetaObject<T> AsQuantumMeta<T>(this T metaData) where T : MetaData
    {
        var quantumMeta = new QuantumMetaObject<T>(metaData);
        return quantumMeta.WithQuantumDecorators();
    }

    /// <summary>
    /// Проверяет, является ли объект декорированным
    /// </summary>
    public static bool IsQuantumDecorated<T>(this T? instance) where T : class
    {
        if (instance == null) return false;
        
        var type = instance.GetType();
        return QuantumTracer.IsInterceptedType(type) || 
               QuantumTracer.GetTypeMetadata(typeof(T)) != null;
    }

    /// <summary>
    /// Получает статистику декораторов для типа
    /// </summary>
    public static TypeMetadata? GetQuantumMetadata<T>(this T instance) where T : class
    {
        return QuantumTracer.GetTypeMetadata(typeof(T));
    }

    /// <summary>
    /// Создает объект напрямую из перехватывающего типа (если существует)
    /// </summary>
    public static T CreateIntercepted<T>(params object[] args) where T : class
    {
        var originalType = typeof(T);
        var interceptedType = QuantumTracer.GetInterceptedType(originalType);
        
        if (interceptedType != originalType)
        {
            // Создаем из перехватывающего типа
            return (T)Activator.CreateInstance(interceptedType, args)!;
        }
        
        // Fallback к обычному созданию с декораторами
        return CreateQuantum<T>(args);
    }
}