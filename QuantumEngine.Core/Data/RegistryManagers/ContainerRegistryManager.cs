#pragma warning disable CS0618 // Type or member is obsolete

using System.Collections.Concurrent;

namespace Engine.Data.RegistryManagers;

using Collections;

internal sealed class ContainerRegistryManager : IRegistryManager<IDataContainer>
{
    private static ConcurrentDictionary<Identifier, IDataContainer> Objects { get; } = new();
    private static readonly Lazy<ContainerRegistryManager> Registry = new(() => new ContainerRegistryManager());

    public static IRegistryManager<IDataContainer> Instance() => Registry.Value;

    public void Register(IDataContainer container)
    {
        // Logger.Info($"Register new {GetType().Name}");
        Objects.TryAdd(container.Id, container);
    }

    public IDataContainer? Get(object id)
    {
        var identifier = Identifier.FromUncertain(id);
        return Objects.GetValueOrDefault(identifier);
    }

    public IDataContainer? Pop(object id)
    {
        var identifier = Identifier.GiveFromUncertain(id);
        if (identifier == null) 
            return null;

        Objects.TryRemove(identifier, out var container);
        return container;
    }

    public int Size => Objects.Count;
    public ICollection<IDataContainer> Values => Objects.Values;
}