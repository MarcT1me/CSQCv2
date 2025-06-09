namespace Engine.Logging;

public class LoggerFormats
{
    public LogFormat Info { get; init; } = new()
    {
        Format = "{0} | {1} | {2} | {3}",
        ColorStart = "\x1b[37m", // Белый
        ColorEnd = "\x1b[0m"
    };

    public LogFormat Debug { get; init; } = new()
    {
        Format = "{0} | {1} | {2} | {3}",
        ColorStart = "\x1b[33m", // Голубой
        ColorEnd = "\x1b[0m"
    };

    public LogFormat Success { get; init; } = new()
    {
        Format = "{0} | {1} | {2} | {3}",
        ColorStart = "\x1b[32m", // Зеленый
        ColorEnd = "\x1b[0m"
    };

    public LogFormat Warning { get; init; } = new()
    {
        Format = "{0} | {1} | {2} | {3}",
        ColorStart = "\x1b[34m", // Желтый
        ColorEnd = "\x1b[0m"
    };

    public LogFormat Error { get; init; } = new()
    {
        Format = "{0} | {1} | {2} | {3}",
        ColorStart = "\x1b[31m", // Красный
        ColorEnd = "\x1b[0m"
    };

    public LogFormat Exception { get; init; } = new()
    {
        Format = "{0} | {1} | {2} | {3}",
        ColorStart = "\x1b[31m", // Красный
        ColorEnd = "\x1b[0m"
    };

    public LogFormat GetFormat(LogLevel level) => level switch
    {
        LogLevel.Info => Info,
        LogLevel.Debug => Debug,
        LogLevel.Success => Success,
        LogLevel.Warning => Warning,
        LogLevel.Error => Error,
        LogLevel.Exception => Exception,
        _ => Info
    };
}