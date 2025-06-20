using Engine.Configuration;

namespace Engine.Asset;

/// <summary>
/// Класс - загрузчик ассетов
/// </summary>
/// <remarks>Работает только после регистрации в типах</remarks>
public abstract class AssetLoader
{
    /// <summary>
    /// Создание данных ассета
    /// </summary>
    /// <param name="assetFile">Данные загрузки ассета</param>
    /// <param name="ct">Остановка задачи</param>
    /// <returns>Загруженные данные</returns>
    public abstract Task<object> LoadFileAsync(
        AssetFile assetFile,
        CancellationToken ct = default
    );

    /// <summary>
    /// Создание объекта данных ассета
    /// </summary>
    /// <param name="assetFile">Данные загрузки ассета</param>
    /// <param name="dependencies">Уже загруженные зависимости ассета</param>
    /// <param name="content">ДанныеЮ загруженные из AssetLoader.LoadFile</param>
    /// <returns>Данные ассета, готовые к упаковки в реестр</returns>
    public static async Task<AssetData> CreateAsset(
        AssetFile assetFile,
        IEnumerable<AssetData>? dependencies,
        object content
    )
    {
        return await Task.FromResult(new AssetData(
            assetFile.TypeName,
            assetFile.Identifier,
            content,
            dependencies: dependencies
        ));
    }

    /// <summary>
    /// Загрузка файла, как текстового
    /// </summary>
    /// <param name="path">Путь до файла</param>
    /// <returns>Текст из файла</returns>
    protected static async Task<string> ReadTextFileAsync(string path)
        => await File.ReadAllTextAsync(path);

    /// <summary>
    /// Загрузка файла, как бинарного
    /// </summary>
    /// <param name="path">Путь до файла</param>
    /// <returns>Бинарные данные файла</returns>
    protected static async Task<byte[]> ReadBinaryFileAsync(string path)
        => await File.ReadAllBytesAsync(path);

    /// <summary>
    /// Открытие файла на чтение
    /// </summary>
    /// <param name="path">Путь до файла</param>
    /// <returns>Поток чтения данных</returns>
    /// <remarks>Не безопасно использовать в контексте AssetLoader</remarks>
    protected static async Task<FileStream> OpenFileAsync(string path)
        => await Task.FromResult(File.OpenRead(path));

    public static string AssetDirectory => Path.Combine(
        EngineCore.RootDirectory,
        BaseConfig.AssetPath
    );
}