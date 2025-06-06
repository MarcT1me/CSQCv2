namespace Engine.Data.Meta;

public class TimedMetaData : MetaData
{
    public DateTime CreatedAt { get; } = DateTime.UtcNow;
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

    public bool IsExpired => ExpiresAt.HasValue && DateTime.UtcNow > ExpiresAt.Value;
}