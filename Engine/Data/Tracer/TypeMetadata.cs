using System.Reflection;
using System.Collections.Concurrent;

namespace Engine.Data.Tracer;

using Meta;

/// <summary>
/// Метаданные типа в системе QuantumTracer
/// </summary>
public class TypeMetadata(Type type) : MetaData
{
    private readonly ConcurrentBag<MethodInfo> _decoratedMethods = new();
    private readonly ConcurrentDictionary<string, object> _customData = new();

    public Type Type { get; } = type;
    public DateTime CreatedAt { get; } = DateTime.UtcNow;
    public IEnumerable<MethodInfo> DecoratedMethods => _decoratedMethods;

    public void AddDecoratedMethod(MethodInfo method)
    {
        _decoratedMethods.Add(method);
    }

    public void SetCustomData(string key, object value)
    {
        _customData[key] = value;
    }

    public T? GetCustomData<T>(string key)
    {
        return _customData.TryGetValue(key, out var value) && value is T typed ? typed : default;
    }

    public int DecoratedMethodCount => _decoratedMethods.Count;
}