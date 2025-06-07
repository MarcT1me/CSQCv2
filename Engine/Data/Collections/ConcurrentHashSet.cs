namespace Engine.Data.Collections;

using System.Collections.Concurrent;

/// <summary>
/// Потокобезопасная реализация HashSet
/// </summary>
public class ConcurrentHashSet<T> : IDisposable where T : notnull
{
    private readonly ConcurrentDictionary<T, byte> _dictionary = new();

    public bool Add(T item) => _dictionary.TryAdd(item, 0);
    public bool Contains(T item) => _dictionary.ContainsKey(item);
    public bool Remove(T item) => _dictionary.TryRemove(item, out _);
    public void Clear() => _dictionary.Clear();
    public int Count => _dictionary.Count;
    public IEnumerable<T> Items => _dictionary.Keys;

    public void Dispose()
    {
        _dictionary.Clear();
        GC.SuppressFinalize(this);
    }
}