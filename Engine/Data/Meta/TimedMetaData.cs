namespace Engine.Data.Meta;

/// <summary>
/// Метаданные хранящие и работающие со временем
/// </summary>
public class TimedMetaData : MetaData
{
    public DateTime CreatedAt { get; private set; } = DateTime.UtcNow;
    public DateTime? ExpiresAt { get; private set; }

    public TimedMetaData(Identifier? identifier = null, float? lifetimeSeconds = null)
        : base(identifier)
    {
        if (lifetimeSeconds.HasValue)
        {
            SetLifetime(lifetimeSeconds.Value);
        }
    }

    public void SetLifetime(float seconds) =>
        ExpiresAt = DateTime.UtcNow.AddSeconds(seconds);

    public void RessetLifetime()
    {
        if (Lifetime != TimeSpan.MaxValue)
        {
            SetLifetime(Lifetime.Seconds);
        }
    }

    public TimeSpan Lifetime => ExpiresAt?.Subtract(CreatedAt) ?? TimeSpan.MaxValue;

    public TimeSpan Age => DateTime.UtcNow - CreatedAt;

    public bool IsExpired => ExpiresAt.HasValue && DateTime.UtcNow > ExpiresAt.Value;
}