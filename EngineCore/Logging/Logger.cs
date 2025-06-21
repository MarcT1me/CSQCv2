using System.Diagnostics;
using Engine.Failures;

namespace Engine.Logging;

using Data.Meta;
using Data.Collections;

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
    private static readonly WritableTale<ILogger> Loggers;
    private static readonly Lock Locker = new();
    private static readonly Lock FileLocker = new();

    static Logger()
    {
        Loggers = new(new("logger-table"));
        Console.OutputEncoding = System.Text.Encoding.UTF8;
        Console.Title = "Quantum console";
        Console.Out.Flush();
    }

    public static void InitLogger()
    {
        AddLogger(new Logger());
        Success("Default Logger added");
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
        return Loggers.Pop(identifier);
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
        foreach (ILogger logger in Loggers.Values)
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
        foreach (ILogger logger in Loggers.Values)
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
        foreach (ILogger logger in Loggers.Values)
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
        foreach (ILogger logger in Loggers.Values)
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
        foreach (ILogger logger in Loggers.Values)
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
        foreach (ILogger logger in Loggers.Values)
        {
            var type = e is FailureException
                ? e.InnerException?.GetType().Name ?? "FailureException"
                : e.GetType().Name;
            logger.Log(
                LogLevel.Exception,
                message + "\n" + e + "\n" + $"{type}: {e.Message}"
            );
        }
    }

    /// <summary>
    /// Вывод пустую строку
    /// </summary>
    public static void Separator()
    {
        foreach (ILogger logger in Loggers.Values)
        {
            logger.LogWithoutFormat("");
        }
    }

    public void LogWithoutFormat(string message) => LogWithoutFormatAsync(message).Wait();

    private Task LogWithoutFormatAsync(string message)
    {
        WriteInConsole(LogLevel.Info, message);
        WriteInFile(message);
        return Task.CompletedTask;
    }

    public void Log(LogLevel level, string message) => LogAsync(level, message).Wait();

    private async Task LogAsync(LogLevel level, string message)
    {
        if (!MetaData.IsActive) return;

        GetCalledMethodName(out var typeName, out var methodName, out var lineNumber);
        var format = MetaData.LogFormat.GetFormat(level);
        var nowTime = DateTime.Now;

        await LogInConsole(
            level, message, format, nowTime,
            typeName, methodName, lineNumber);
        await LogInFile(
            level, message, format, nowTime,
            typeName, methodName, lineNumber
        );
    }

    private async Task LogInConsole(
        LogLevel level, string message, LogFormat format, DateTime nowTime,
        string typeName, string methodName, int lineNumber
    )
    {
        var formattedMessage = string.Format(
            format.ColorizedFormat ?? format.Format,
            nowTime,
            nowTime.Millisecond * 10 + float.Round(nowTime.Nanosecond / 100f, 0),
            level,
            typeName,
            methodName,
            lineNumber,
            message
        );

        await WriteInConsole(level, formattedMessage);
    }

    private Task WriteInConsole(LogLevel level, string message)
    {
        var writer = level is LogLevel.Error or LogLevel.Exception ? Console.Error : Console.Out;
        lock (Locker)
        {
            writer.WriteLineAsync(message);
            writer.FlushAsync();
        }

        return Task.CompletedTask;
    }

    private async Task LogInFile(
        LogLevel level, string message, LogFormat format, DateTime nowTime,
        string typeName, string methodName, int lineNumber)
    {
        if (MetaData is { IsExpired: true, File: not null })
        {
            MetaData.File = LoggerFile.GetOrCreate(MetaData.File?.Info.Name);
            MetaData.RessetLifetime();
        }

        var formattedMessage = string.Format(
            format.Format,
            nowTime,
            nowTime.Millisecond * 10 + float.Round(nowTime.Nanosecond / 100f, 0),
            level,
            typeName,
            methodName,
            lineNumber,
            message
        );

        await WriteInFile(formattedMessage);
    }

    private Task WriteInFile(string message)
    {
        if (MetaData.File is not { } file) return Task.CompletedTask;
        lock (FileLocker)
        {
            file.Writer.WriteLineAsync(message);
            file.Writer.FlushAsync();
        }

        return Task.CompletedTask;
    }

    private static void GetCalledMethodName(
        out string typeName, out string methodName, out int lineNumber
    )
    {
        var stackTrace = new StackTrace(skipFrames: 6, fNeedFileInfo: true);
#if DEBUG
        var frame = stackTrace.GetFrame(0);
#else
        var frame = stackTrace.GetFrame(1);
#endif
        if (frame == null)
        {
            typeName = "UnknownType";
            methodName = "UnknownMethod";
            lineNumber = -1;
            return;
        }

        var method = frame.GetMethod();
        if (method == null)
        {
            typeName = "UnknownType";
            methodName = "UnknownMethod";
            lineNumber = -1;
            return;
        }

        typeName = method.DeclaringType?.FullName ?? "UnknownType";
        methodName = method.Name;
        lineNumber = frame.GetFileLineNumber();
    }

    public void Dispose()
    {
        PopLogger(Id);
        GC.SuppressFinalize(this);
    }

    ~Logger() => Dispose();
}