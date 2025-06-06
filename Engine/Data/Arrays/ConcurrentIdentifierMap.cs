using System.Collections.Concurrent;

namespace Engine.Data.Arrays;

public class ConcurrentIdentifierMap<T> : ConcurrentDictionary<Identifier, T>;