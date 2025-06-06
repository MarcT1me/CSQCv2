namespace Engine.Threading;

using Configuration;
using Data.Meta;
using Failures;

public class QThreadMeta(
    string? name,
    FailureLevel? failureLevel,
    bool isBackground,
    float? lifetimeSeconds
) : TimedMetaData(name, lifetimeSeconds ?? ThreadingConfig.LifetimeSeconds)
{
    public FailureLevel FailureLevel { get; } = failureLevel ?? ThreadingConfig.BaseFailureLevel;
    public bool IsBackground { get; } = isBackground;
}