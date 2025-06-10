using System.Collections.Concurrent;

namespace Engine.Data.Collections;

public class ConcurrentIdentifierMap<T>(Dictionary<Identifier, T>? data)
    : ConcurrentDictionary<Identifier, T>(data ?? new Dictionary<Identifier, T>());