namespace Engine.Failures;

using Data;

public class FailureException : Exception
{
    public FailureException(string message) : base(message)
    {
    }

    public FailureException(string message, Exception innerException) : base(message, innerException)
    {
    }

    public Identifier? CatchId { get; init; }
    public FailureLevel Level { get; init; }
    public DateTime Timestamp { get; } = DateTime.UtcNow;

    public override string ToString() =>
        $"FailureException<{CatchId?.ToString() ?? "N/A"}>: {Message} " +
        $"(Critical: {Level}, Timestamp: {Timestamp:O})";
}