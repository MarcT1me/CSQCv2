namespace QuantumCore.Threading;

using Failures;

public class ThreadException : FailureException
{
    public ThreadException(string message) : base(message)
    {
    }

    public ThreadException(string message, Exception innerException) : base(message, innerException)
    {
    }
}

public sealed class AlreadyExistThreadException : ThreadException
{
    public AlreadyExistThreadException(string id
    ) : base($"Thread with id: '{id}' already exists")
    {
    }

    public AlreadyExistThreadException(string id, Exception innerException
    ) : base($"Thread with id: '{id}' already exists", innerException)
    {
    }
}

public sealed class PendingThreadNotExistException : ThreadException
{
    public PendingThreadNotExistException(string id
    ) : base($"Pending thread with id: '{id}' doesn't exist")
    {
    }

    public PendingThreadNotExistException(string id, Exception innerException
    ) : base($"Pending thread with id: '{id}' doesn't exist", innerException)
    {
    }
}

public sealed class ThreadReleaseException : ThreadException
{
    public ThreadReleaseException(string id
    ) : base($"Failed to release thread with id: '{id}'")
    {
    }

    public ThreadReleaseException(string id, Exception innerException
    ) : base($"Failed to release thread with id: '{id}'", innerException)
    {
    }
}