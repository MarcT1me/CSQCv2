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

    public FailureLevel Level { get; init; } = FailureLevel.Second;
    public Identifier? CatchId { get; set; }
    public DateTime Timestamp { get; } = DateTime.UtcNow;

    public override string ToString() =>
        $"FailureException<{CatchId?.ToString() ?? "N/A"}>(Critical: {Level}, Timestamp: {Timestamp:O}): {Message}\n" +
        $"Traceback:\n" +
        $"{base.ToString()}";

    public void Handle()
    {
        if (Level is FailureLevel.Second)
        {
            Logger.Exception("Second level exception handling", this);
            Logger.Separator();
            
            Console.WriteLine("Full Traceback (FailureException.Handle):");
            throw this;
        }
    }
}