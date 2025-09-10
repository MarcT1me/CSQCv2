namespace QuantumCore.Time.Defers;

using Data.Meta;

public class DeferMeta(Clock attachClock, Action callback, bool disposable = true, float? lifetime = null)
    : TimedMetaData(lifetimeSeconds: lifetime)
{
    public Clock AttachClock { get; } = attachClock;
    public Action Callback { get; } = callback;
    public bool Disposable { get; } = disposable;
}