namespace QuantumCore.Logging;

public sealed class LogFormat
{
    public required string Format { get; init; }
    public string? ColorizedFormat { get; init; } // ANSI-код сброса цвета
}