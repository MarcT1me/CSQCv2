namespace Engine.Data.Meta;

/// <summary>
/// Объект движка
/// </summary>
/// <param name="metaData">Метаданные объекта</param>
/// <typeparam name="T">Тип метаданных</typeparam>
public class MetaObject<T>(T metaData) where T : MetaData
{
    public T MetaData { get; } = metaData;

    public Identifier Id => MetaData.Identifier;
}