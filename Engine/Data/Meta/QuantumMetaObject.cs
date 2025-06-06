namespace Engine.Data.Meta;

using Decorators;
using Tracer;

/// <summary>
/// Расширенный MetaObject с поддержкой QuantumTracer
/// </summary>
public class QuantumMetaObject<T> : MetaObject<T> where T : MetaData
{
    private readonly TypeMetadata? _typeMetadata;

    public QuantumMetaObject(T metaData) : base(metaData)
    {
        _typeMetadata = QuantumTracer.GetTypeMetadata(GetType());
        
        // Автоматически регистрируемся в QuantumTracer если есть декорированные методы
        if (_typeMetadata == null && HasDecoratedMethods())
        {
            QuantumTracer.RegisterDecoratedType(GetType());
        }
    }

    /// <summary>
    /// Создает проксированную версию объекта
    /// </summary>
    public QuantumMetaObject<T> WithQuantumDecorators()
    {
        return QuantumDecoratorFactory.CreateIfNeeded(this);
    }

    /// <summary>
    /// Получает метаданные типа из QuantumTracer
    /// </summary>
    public TypeMetadata? GetQuantumMetadata() => _typeMetadata;

    private bool HasDecoratedMethods()
    {
        return GetType().GetMethods(System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.Instance)
            .Any(m => m.GetCustomAttributes(false).OfType<IDecoratorAttribute>().Any());
    }
}