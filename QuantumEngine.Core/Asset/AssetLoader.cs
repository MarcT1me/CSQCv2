using System.Text;

namespace QuantumCore.Asset;

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
    /// <param name="dependencies">Уже загруженные зависимости ассета</param>
    /// <returns>Загруженные данные</returns>
    public abstract object LoadFile(
        AssetFile assetFile,
        IEnumerable<AssetData> dependencies
    );

    /// <summary>
    /// Создание объекта данных ассета
    /// </summary>
    /// <param name="assetFile">Данные загрузки ассета</param>
    /// <param name="dependencies">Уже загруженные зависимости ассета</param>
    /// <param name="content">ДанныеЮ загруженные из AssetLoader.LoadFile</param>
    /// <returns>Данные ассета, готовые к упаковки в реестр</returns>
    public virtual AssetData CreateAsset(
        AssetFile assetFile,
        IEnumerable<AssetData> dependencies,
        object content
    )
    {
        return new AssetData(
            assetFile.TypeName,
            assetFile.Identifier,
            content,
            dependencies: dependencies
        );
    }

    /// <summary>
    /// Загрузка ассета, как текстового
    /// </summary>
    /// <param name="assetFile">Данные об ассете</param>
    /// <param name="encoding">Кодировка текста ассета</param>
    /// <returns>Текстовые данные ассета</returns>
    public static string ReadTextAsset(AssetFile assetFile, Encoding? encoding = null)
    {
        using var stream = assetFile.GetStream();
        using var reader = new StreamReader(stream, encoding ?? Encoding.UTF8);
        return reader.ReadToEnd();
    }

    /// <summary>
    /// Загрузка ассета, как байтового массива
    /// </summary>
    /// <param name="assetFile">Данные об ассете</param>
    /// <returns>Байты данных ассета</returns>
    public static byte[] ReadBinaryAsset(AssetFile assetFile)
    {
        using var stream = assetFile.GetStream();
        using var memoryStream = new MemoryStream();
        stream.CopyTo(memoryStream);
        return memoryStream.ToArray();
    }

    /// <summary>
    /// Загрузка потока, как текстового
    /// </summary>
    /// <param name="assetStream">Поток данных ассета</param>
    /// <param name="encoding">Кодировка текста ассета</param>
    /// <returns>Текстовые данные ассета</returns>
    public static string ReadTextStream(Stream assetStream, Encoding? encoding = null)
    {
        using var reader = new StreamReader(assetStream, encoding ?? Encoding.UTF8);
        return reader.ReadToEnd();
    }

    /// <summary>
    /// Загрузка потока, как байтового массива
    /// </summary>
    /// <param name="assetStream">Поток данных ассета</param>
    /// <returns>Байты данных ассета</returns>
    public static byte[] ReadBinaryStream(Stream assetStream)
    {
        using var memoryStream = new MemoryStream();
        assetStream.CopyTo(memoryStream);
        return memoryStream.ToArray();
    }

    /// <summary>
    /// Загрузка файла, как текстового
    /// </summary>
    /// <param name="path">Путь до файла</param>
    /// <returns>Текст из файла</returns>
    public static string ReadTextFile(string path) => File.ReadAllText(path);

    /// <summary>
    /// Загрузка файла, как бинарного
    /// </summary>
    /// <param name="path">Путь до файла</param>
    /// <returns>Бинарные данные файла</returns>
    public static byte[] ReadBinaryFile(string path) => File.ReadAllBytes(path);

    /// <summary>
    /// Открытие файла на чтение
    /// </summary>
    /// <param name="path">Путь до файла</param>
    /// <returns>Поток чтения данных</returns>
    /// <remarks>Не безопасно использовать в контексте AssetLoader</remarks>
    public static FileStream OpenFile(string path) => File.OpenRead(path);

    /// <summary>
    /// Полный путь до папки ассетов
    /// </summary>
    public static string AssetDirectory => Path.Combine(
        Core.RootDirectory,
        Core.CoreConfiguration.Get<string>("app.asset.dir")
    );
}