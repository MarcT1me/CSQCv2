using System.Collections.Concurrent;

namespace QuantumCore.Data.Collections;

public sealed class ConcurrentIdentifierMap<T>(Dictionary<Identifier, T>? data)
    : ConcurrentDictionary<Identifier, T>(data ?? new Dictionary<Identifier, T>());