using Engine.Data.Collections;

namespace Engine.Data.RegistryManagers;

using Graphic.Window;

public class WindowRegistryManager : IRegistryManager<Window>
{
    private static ConcurrentIdentifierMap<Window> Windows { get; } = new([]);
    private static readonly Lazy<WindowRegistryManager> Registry = new(() => new WindowRegistryManager());

    public static IRegistryManager<Window> Instance() => Registry.Value;

    public void Register(Window window)
    {
        Windows.TryAdd(window.Id, window);
    }

    public Window? Get(object id)
    {
        var identifier = Identifier.FromUncertain(id);
        return Windows.GetValueOrDefault(identifier);
    }

    public Window? Pop(object id)
    {
        var identifier = Identifier.FromUncertain(id);
        Windows.Remove(identifier, out var window);
        return window;
    }

    public int Size => Windows.Count;
    public ICollection<Window> Values => Windows.Values;
}