#pragma warning disable CS0618 // Type or member is obsolete

namespace Engine.Data.Collections;

using Meta;

/// <summary>
/// Имутабельный контейнер с данными
/// </summary>
/// <param name="metaData">Метаданные массива</param>
/// <param name="data">Данные на сохранение</param>
/// <typeparam name="T">Тип данных контейнера</typeparam>
public sealed class Table<T>(MetaData metaData, IEnumerable<KeyValuePair<Identifier, T>> data)
    : DataContainer<T>(metaData, data);