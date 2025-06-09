using System.Diagnostics;

namespace Engine.Logging;

using Data.Meta;

/// <summary>
/// Класс логирования
/// </summary>
/// <param name="filePath">Путь до файла</param>
/// <param name="logFormats">Форматы логирования</param>
/// <param name="timeDuration">Время жизни файла</param>
public sealed class Logger(
    string? filePath = null,
    LoggerFormats? logFormats = null,
    float? timeDuration = null
) : MetaObject<LoggerMeta>(
    new LoggerMeta(
        filePath: filePath,
        logFormats: logFormats,
        timeDuration: timeDuration
    )
), ILogger, IDisposable
{
    private static readonly LoggersTable Loggers;

    static Logger()
    {
        Console.OutputEncoding = System.Text.Encoding.UTF8;
        Loggers = new LoggersTable();
        AddLogger(new Logger());
        Logger.Success("Default Logger added");
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
            logger.Log(LogLevel.Error, message);
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
            logger.LogWithoutFormat($"{e.GetType().Name}: {e.Message}");
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
        var format = MetaData.LogFormat.GetFormat(level);
        var formattedMessage = string.Format(
            format.Format,
            DateTime.Now,
            level,
            GetMethodName(),
            message
        );
    
        // Добавляем цветовые коды
        Console.WriteLine($"{format.ColorStart}{formattedMessage}{format.ColorEnd}");
    }

    private void LogInFile(LogLevel level, string message)
    {
        if (MetaData is { IsExpired: true, File: not null })
        {
            MetaData.File = LoggerFile.GetOrCreate(MetaData.File?.Info.Name);
            MetaData.RessetLifetime();
        }

        MetaData.File?.Writer.WriteLine(
            MetaData.LogFormat.GetFormat(level).Format,
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