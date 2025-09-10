namespace Engine.Objects;

public interface IHasActorStatus
{
    ObjectStatusFlags ObjectStatus { get; }
    
    public bool IsActive => ObjectStatus.HasFlag(ObjectStatusFlags.Active);
    public bool IsDynamic => ObjectStatus.HasFlag(ObjectStatusFlags.Dynamic);
    public bool IsVisible => IsActive || ObjectStatus.HasFlag(ObjectStatusFlags.Visible);
}