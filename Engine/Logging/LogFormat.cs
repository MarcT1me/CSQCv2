namespace Engine.Logging;

public class LogFormat
{
    public required string Format { get; init; }
    public required string ColorStart { get; init; } // ANSI-код начала цвета
    public required string ColorEnd { get; init; } // ANSI-код сброса цвета
}