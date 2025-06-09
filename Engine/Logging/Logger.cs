using System.Diagnostics;

namespace Engine.Logging;

using Data.Meta;

/// <summary>
/// Данные о файле для логирования
/// </summary>
/// <param name="info">Информация о файле</param>
/// <param name="writer">Писатель в файл</param>
public class LoggerFile(FileInfo info, StreamWriter writer) : IDisposable
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

        var fullpath = Path.Join(EngineCore.RootDirectory, filePath);
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

/// <summary>
/// Данные класса для логирования
/// </summary>
/// <param name="filePath">Путь до файла</param>
/// <param name="logFormat">Формат логирования</param>
/// <param name="isActive">Статус</param>
/// <param name="timeDuration">Время жизни файла</param>
public class LoggerMeta(
    string? filePath = null,
    string? logFormat = null,
    bool isActive = true,
    float? timeDuration = null
) : TimedMetaData(
    lifetimeSeconds: timeDuration
)
{
    public bool IsActive = isActive;
    public LoggerFile? File = LoggerFile.GetOrCreate(filePath);
    public readonly string LogFormat = logFormat ?? "{0} | {1} | {2} | {3}";
}

/// <summary>
/// Класс логирования
/// </summary>
/// <param name="filePath">Путь до файла</param>
/// <param name="logFormat">Формат логирования</param>
/// <param name="timeDuration">Время жизни файла</param>
public sealed class Logger(
    string? filePath = null,
    string? logFormat = null,
    float? timeDuration = null
) : MetaObject<LoggerMeta>(
    new LoggerMeta(
        filePath: filePath,
        logFormat: logFormat,
        timeDuration: timeDuration
    )
), ILogger, IDisposable
{
    private static readonly LoggersTable Loggers;

    static Logger()
    {
        Loggers = new LoggersTable(new MetaData("logger-table"));
        AddLogger(new Logger());
    }

    /// <summary>
    /// Добавляет логер в список
    /// </summary>
    /// <param name="logger">Сам логер</param>
    public static void AddLogger(ILogger logger)
    {
        Loggers.Set(logger.Id, logger);
    }

    /// <summary>
    /// Выдаёт логер по идентификатору
    /// </summary>
    /// <param name="identifier">Объект, ассоциирующеюся с логером</param>
    /// <returns>Логер, если такой нашёлся</returns>
    public static ILogger? GetLogger(object identifier)
    {
        return Loggers.Get(identifier) as ILogger;
    }

    /// <summary>
    /// Изымает логер из списка зарегестрированных
    /// </summary>
    /// <param name="identifier">Объект, ассоциирующеюся с логером</param>
    /// <returns>Логер, если такой нашёлся</returns>
    public static ILogger? PopLogger(object identifier)
    {
        return Loggers.Pop(identifier) as ILogger;
    }

    /// <summary>
    /// Запускает логирование
    /// </summary>
    public void Start()
    {
        MetaData.IsActive = true;
        Debug($"Logging started. File {MetaData.File?.Info.Name}");
    }

    /// <summary>
    /// Останавливает логирование
    /// </summary>
    public void Finish()
    {
        Debug($"Logging finished. File {MetaData.File?.Info.Name}");
        MetaData.IsActive = false;
    }

    /// <summary>
    /// Информационный лог
    /// </summary>
    /// <param name="message">Текст с информацией</param>
    public static void Info(string message)
    {
        foreach (var logger in Loggers.Values)
        {
            logger.Log(LogLevel.Info, message);
        }
    }

    /// <summary>
    /// Отладочный лог
    /// </summary>
    /// <param name="message">Сообщение</param>
    public static void Debug(string message)
    {
        foreach (var logger in Loggers.Values)
        {
            logger.Log(LogLevel.Debug, message);
        }
    }

    /// <summary>
    /// Лог об удачной операции
    /// </summary>
    /// <param name="message">Сообщение</param>
    public static void Success(string message)
    {
        foreach (var logger in Loggers.Values)
        {
            logger.Log(LogLevel.Success, message);
        }
    }

    /// <summary>
    /// Лог - предупреждение
    /// </summary>
    /// <param name="message">Текст предупреждения</param>
    public static void Warning(string message)
    {
        foreach (var logger in Loggers.Values)
        {
            logger.Log(LogLevel.Warning, message);
        }
    }

    /// <summary>
    /// Сообщение об ошибке
    /// </summary>
    /// <param name="message">Сообщение о ошибке</param>
    public static void Error(string message)
    {
        foreach (var logger in Loggers.Values)
        {
            logger.Log(LogLevel.Warning, message);
        }
    }

    /// <summary>
    /// Вывод ошибки
    /// </summary>
    /// <param name="message">Сообщение в добавок к ошибке</param>
    /// <param name="e">Обрабатываемая ошибка</param>
    public static void Exception(string message, Exception e)
    {
        foreach (var logger in Loggers.Values)
        {
            logger.Log(LogLevel.Exception, message);
            if (e.StackTrace != null) logger.LogWithoutFormat(e.StackTrace);
            if (e.Source != null) logger.LogWithoutFormat(e.Source);
            logger.LogWithoutFormat($"{e.GetType()}: {e.Message}");
        }
    }

    public void LogWithoutFormat(string message)
    {
        Console.WriteLine(message);
        MetaData.File?.Writer.WriteLine(message);
    }

    public void Log(LogLevel level, string message)
    {
        if (!MetaData.IsActive) return;
        LogInConsole(level, message);
        LogInFile(level, message);
    }

    private void LogInConsole(LogLevel level, string message)
    {
        Console.WriteLine(
            MetaData.LogFormat,
            DateTime.Now,
            level,
            GetMethodName(),
            message
        );
    }

    private void LogInFile(LogLevel level, string message)
    {
        if (MetaData.IsExpired & MetaData.File is not null)
        {
            MetaData.File = LoggerFile.GetOrCreate(MetaData.File?.Info.Name);
        }

        MetaData.File?.Writer.WriteLine(
            MetaData.LogFormat,
            DateTime.Now,
            level,
            GetMethodName(),
            message
        );
    }

    private static string GetMethodName()
    {
        var stackTrace = new StackTrace(skipFrames: 2, fNeedFileInfo: true);
        var frame = stackTrace.GetFrame(0);

        if (frame == null) return "UnknownType:UnknownMethod";

        var method = frame.GetMethod();

        if (method == null) return "UnknownType:UnknownMethod";

        var callerType = method.DeclaringType?.FullName ?? "UnknownType";
        var callerMethod = method.Name;

        return $"{callerType}:{callerMethod}";
    }

    public void Dispose()
    {
        PopLogger(Id);
        GC.SuppressFinalize(this);
    }

    ~Logger() => Dispose();
}