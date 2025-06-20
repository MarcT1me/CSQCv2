namespace Engine.Data.Meta;

public delegate void ReloadDataEvent(MetaData data);

public delegate void OnExitEvent();

/// <summary>
/// Объект движка
/// </summary>
/// <param name="metaData">Метаданные объекта</param>
/// <typeparam name="T">Тип метаданных</typeparam>
public class MetaObject<T>(T metaData) where T : MetaData
{
    public static event ReloadDataEvent? ReloadingData;
    public static event OnExitEvent? ExitHandling;

    public T MetaData { get; init; } = metaData;

    public Identifier Id => MetaData.Identifier;

    public static void HandleReloadDataEvent(MetaData data)
    {
        ReloadingData?.Invoke(data);
    }
    
    public static void HandleExitEvent()
    {
        ExitHandling?.Invoke();
    }
}