namespace Engine.Failures;

using Data.Meta;
using Data;

public sealed class CatchMeta(
    object? identifier,
    FailureLevel failureLevel,
    IFailureHandler? handler
) : TimedMetaData(Identifier.FromUncertain(identifier))
{
    public IFailureHandler? Handler { get; } = handler;
    public FailureLevel FailureLevel { get; } = failureLevel;

    public FailuresRoster Failures { get; } = new(
        new MetaData(
            $"failuresRoster-{identifier}"
        )
    );
}