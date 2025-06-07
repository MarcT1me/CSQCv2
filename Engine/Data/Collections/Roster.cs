#pragma warning disable CS0618 // Type or member is obsolete

using System.Collections.Concurrent;

namespace Engine.Data.Collections;

using Meta;

/// <summary>
/// Контейнер, хранящий данные в древовидной структуре
/// </summary>
/// <typeparam name="T">Тип хранимых данных</typeparam>
public class Roster<T> : DataContainer<T>
{
    /// <summary>
    /// Ветки массива
    /// </summary>
    public ConcurrentDictionary<Identifier, Roster<T>> Branches { get; } = [];

    public Roster(MetaData metaData)
        : base(metaData)
    {
    }

    public Roster(MetaData metaData, IEnumerable<KeyValuePair<Identifier, T>> data)
        : base(metaData, data)
    {
    }

    #region Branch creating operations

    /// <summary>
    /// Создание новой ветки в структуре (сохраняется в Branch)
    /// </summary>
    /// <param name="metaData">Метаданные новой ветки</param>
    /// <returns>Новая ветка</returns>
    public Roster<T> NewBranch(MetaData metaData) => NewBranch(metaData, new List<KeyValuePair<Identifier, T>>());

    /// <summary>
    /// Создание новой ветки в структуре (сохраняется в Branch)
    /// </summary>
    /// <param name="metaData">Метаданные новой ветки</param>
    /// <param name="data">Данные из которых будет создана ветка</param>
    /// <returns>Новая ветка</returns>
    public Roster<T> NewBranch(MetaData metaData, IEnumerable<KeyValuePair<Identifier, T>> data)
    {
        var branch = new Roster<T>(metaData, data);
        SaveBranch(branch);
        return branch;
    }

    /// <summary>
    /// Сохраняет ветку
    /// </summary>
    /// <param name="branch">Ветка на сохранение</param>
    public void SaveBranch(Roster<T> branch)
    {
        Branches[branch.Id] = branch;
    }

    /// <summary>
    /// Выдаёт ветку
    /// </summary>
    /// <param name="id">Неуверенный идентификатор ветки в структуре</param>
    /// <returns>Ветка, если есть</returns>
    public Roster<T>? GetBranch(object id)
    {
        Identifier? identifier = Identifier.GiveFromUncertain(id);
        return identifier == null ? null : Branches.GetValueOrDefault(identifier);
    }

    #endregion

    public override void Set(object? key, T? value)
    {
        if (Identifier.GiveFromUncertain(key) is not { } identifier) return;

        if (value == null)
        {
            Data.TryRemove(identifier, out _);
        }
        else
        {
            Data[identifier] = value;
        }
    }
}