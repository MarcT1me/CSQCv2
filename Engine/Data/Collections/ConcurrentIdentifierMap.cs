using System.Collections.Concurrent;

namespace Engine.Data.Collections;

public class ConcurrentIdentifierMap<T> : ConcurrentDictionary<Identifier, T>;