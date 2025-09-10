namespace QuantumCore.Failures;

using Data;
using Logging;

public class FailureException : Exception
{
    /// <summary>
    /// Create Failure from base Exception
    /// </summary>
    /// <param name="exception">Exception for creating a Failure</param>
    public FailureException(Exception exception) : this(exception.Message)
    {
    }

    /// <summary>
    /// Create Failure from message
    /// </summary>
    /// <param name="message">Failure message</param>
    public FailureException(string message) : base(message)
    {
    }

    /// <summary>
    /// Create Failure from base Exception and inner exception
    /// </summary>
    /// <param name="exception">Base error for creating a Failure</param>
    /// <param name="innerException">Inner Exception</param>
    public FailureException(Exception exception, Exception innerException) : this(exception.Message, innerException)
    {
    }

    /// <summary>
    ///  Create Failure from message and inner exception
    /// </summary>
    /// <param name="message">Failure message</param>
    /// <param name="innerException">Inner Exception</param>
    public FailureException(string message, Exception innerException) : base(message, innerException)
    {
    }

    /// <summary>
    /// Failure Level
    /// </summary>
    public FailureLevel Level { get; init; } = FailureLevel.Second;

    /// <summary>
    /// processed the Catch
    /// </summary>
    public Identifier? CatchId { get; set; }

    /// <summary>
    /// Just failure Time-stamp
    /// </summary>
    public DateTime Timestamp { get; } = DateTime.UtcNow;

    public override string ToString() =>
        $"FailureException<{CatchId?.ToString() ?? "N/A"}>(" +
        $"Critical: {Level}, " +
        $"Timestamp: {Timestamp:O}): {Message}\n" +
        $"Traceback:\n" +
        $"{base.ToString()}";

    /// <summary>
    /// Re-cast if failure has Second level
    /// </summary>
    /// <exception cref="FailureException">Re-casted exception</exception>
    public void RecastIfSecond()
    {
        if (Level is FailureLevel.Second)
        {
            Logger.Exception("Second level exception handling", this);
            Logger.Separator();
            throw this;
        }
    }
}