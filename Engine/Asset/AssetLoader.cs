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
    /// <returns>Загруженные данные</returns>
    public abstract object LoadFile(AssetFile assetFile);

    /// <summary>
    /// Создание объекта данных ассета
    /// </summary>
    /// <param name="assetFile">Данные загрузки ассета</param>
    /// <param name="dependencies">Уже загруженные зависимости ассета</param>
    /// <param name="content">ДанныеЮ загруженные из AssetLoader.LoadFile</param>
    /// <returns>Данные ассета, готовые к упаковки в реестр</returns>
    public abstract AssetData CreateAsset(
        AssetFile assetFile,
        IEnumerable<AssetData>? dependencies,
        object content
    );

    /// <summary>
    /// Загрузка файла, как текстового
    /// </summary>
    /// <param name="path">Путь до файла</param>
    /// <returns>Текст из файла</returns>
    protected static string ReadTextFile(string path)
    {
        return File.ReadAllText(path);
    }

    /// <summary>
    /// Загрузка файла, как бинарного
    /// </summary>
    /// <param name="path">Путь до файла</param>
    /// <returns>Бинарные данные файла</returns>
    protected static byte[] ReadBinaryFile(string path)
    {
        return File.ReadAllBytes(path);
    }

    /// <summary>
    /// Открытие файла на чтение
    /// </summary>
    /// <param name="path">Путь до файла</param>
    /// <returns>Поток чтения данных</returns>
    /// <remarks>Не безопасно использовать в контексте AssetLoader</remarks>
    protected static Stream OpenFile(string path)
    {
        return File.OpenRead(path);
    }

    public static string AssetDirectory => Path.Combine(
        EngineCore.RootDirectory,
        BaseConfig.AssetPath
    );
}