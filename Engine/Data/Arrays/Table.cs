#pragma warning disable CS0618 // Type or member is obsolete

namespace Engine.Data.Arrays;

using Meta;

public sealed class Table<T>(MetaData metaData, IEnumerable<KeyValuePair<Identifier, T>> data)
    : DataContainer<T>(metaData, data);