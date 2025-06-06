namespace Engine.Data.Meta;

public class MetaObject<T>(T metaData) where T : MetaData
{
    public T MetaData { get; } = metaData;

    public Identifier Id => MetaData.Identifier;
}