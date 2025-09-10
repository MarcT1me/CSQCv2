namespace QuantumCore.Logging;

/// <summary>
/// Данные о файле для логирования
/// </summary>
/// <param name="info">Информация о файле</param>
/// <param name="writer">Писатель в файл</param>
public sealed class LoggerFile(FileInfo info, StreamWriter writer) : IDisposable
{
    private static LoggerFile? _instance;

    public FileInfo Info = info;
    public StreamWriter Writer = writer;

    /// <summary>
    /// Выдаёт существующий или создаёт новый файл для логирования
    /// </summary>
    /// <param name="filePath">Путь до файла</param>
    /// <returns></returns>
    public static LoggerFile? GetOrCreate(string? filePath)
    {
        if (_instance != null || filePath is null) return _instance;

        var fullpath = Path.Join(Core.RootDirectory, filePath);
        var file = new FileInfo(fullpath);
        var fileWriter = new StreamWriter(
            new FileStream(fullpath, FileMode.Open, FileAccess.Write)
        );
        fileWriter.AutoFlush = true;
        _instance = new LoggerFile(file, fileWriter);

        return _instance;
    }

    public void Dispose()
    {
        Writer.FlushAsync();
        Writer.Close();
        Writer.Dispose();
        GC.SuppressFinalize(this);
    }

    ~LoggerFile() => Dispose();
}