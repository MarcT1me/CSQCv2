namespace Engine.Objects;

public interface IHasActorStatus
{
    ObjectStatusFlags ObjectStatus { get; }
    
    public bool IsActive() => ObjectStatus.HasFlag(ObjectStatusFlags.Active);
}