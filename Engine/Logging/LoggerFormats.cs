namespace Engine.Logging;

public class LoggerFormats
{
    public LogFormat Info { get; init; } = new()
    {
        Format = "{0}:{1} | {2} | {3}:{4} - {5}",
        ColorizedFormat = "\x1b[32m{0}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[2;32m{1}\x1b[0m " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[1;38m{2}\x1b[0m      " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[36m{3}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[36m{4}\x1b[0m " +
                          
                          "\x1b[31m-\x1b[0m " +
                          
                          "\x1b[1;38m{5}\x1b[0m"
    };

    public LogFormat Debug { get; init; } = new()
    {
        Format = "{0}:{1} | {2} | {3}:{4} - {5}",
        ColorizedFormat = "\x1b[32m{0}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[2;32m{1}\x1b[0m " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[1;33m{2}\x1b[0m     " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[36m{3}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[36m{4}\x1b[0m " +
                          
                          "\x1b[31m-\x1b[0m " +

                          "\x1b[1;33m{5}\x1b[0m"
    };

    public LogFormat Success { get; init; } = new()
    {
        Format = "{0}:{1} | {2} | {3}:{4} - {5}",
        ColorizedFormat = "\x1b[32m{0}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[2;32m{1}\x1b[0m " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[1;32m{2}\x1b[0m   " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[36m{3}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[36m{4}\x1b[0m " +
                          
                          "\x1b[31m-\x1b[0m " +
                          
                          "\x1b[1;32m{5}\x1b[0m"
    };

    public LogFormat Warning { get; init; } = new()
    {
        Format = "{0}:{1} | {2} | {3}:{4} - {5}",
        ColorizedFormat = "\x1b[32m{0}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[2;32m{1}\x1b[0m " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[1;34m{2}\x1b[0m   " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[36m{3}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[36m{4}\x1b[0m " +
                          
                          "\x1b[31m-\x1b[0m " +

                          "\x1b[1;34m{5}\x1b[0m"
    };

    public LogFormat Error { get; init; } = new()
    {
        Format = "{0}:{1} | {2} | {3}:{4} - {5}",
        ColorizedFormat = "\x1b[32m{0}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[2;32m{1}\x1b[0m " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[1;31m{2}\x1b[0m     " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[36m{3}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[36m{4}\x1b[0m " +
                          
                          "\x1b[31m-\x1b[0m " +

                          "\x1b[1;38m{5}\x1b[0m"
    };

    public LogFormat Exception { get; init; } = new()
    {
        Format = "{0}:{1} | {2} | {3}:{4} - {5}",
        ColorizedFormat = "\x1b[32m{0}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[2;32m{1}\x1b[0m " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[1;31m{2}\x1b[0m " +
                          
                          "\x1b[31m|\x1b[0m " +
                          
                          "\x1b[36m{3}\x1b[0m" +
                          "\x1b[31m:\x1b[0m" +
                          "\x1b[36m{4}\x1b[0m " +
                          
                          "\x1b[31m-\x1b[0m " +

                          "\x1b[1;31m{5}\x1b[0m"
    };

    public LogFormat GetFormat(LogLevel level) => level switch
    {
        LogLevel.Info => Info,
        LogLevel.Debug => Debug,
        LogLevel.Success => Success,
        LogLevel.Warning => Warning,
        LogLevel.Error => Error,
        LogLevel.Exception => Exception,
        _ => throw new ArgumentOutOfRangeException(nameof(level), level, null)
    };
}