#pragma warning disable CS0618 // Type or member is obsolete

namespace QuantumCore.Data.Collections;

using Meta;

/// <summary>
/// Имутабельный контейнер с данными
/// </summary>
/// <param name="metaData">Метаданные массива</param>
/// <param name="data">Данные на сохранение</param>
/// <typeparam name="T">Тип данных контейнера</typeparam>
public sealed class Table<T>(MetaData metaData, Dictionary<Identifier, T> data)
    : DataContainer<T>(metaData, data);