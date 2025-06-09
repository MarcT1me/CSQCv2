namespace Engine.Failures;

using Data;
using Logging;

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

    public void Handle(string message = "exception has Second level => rethrow")
    {
        if (Level == FailureLevel.Second)
            Logger.Exception(message, this);

        if (Level is FailureLevel.Second)
        {
            throw this;
        }
    }
}