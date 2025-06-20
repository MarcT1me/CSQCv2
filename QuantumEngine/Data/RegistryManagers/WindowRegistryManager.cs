using System.Collections.Concurrent;

namespace Engine.Data.RegistryManagers;

using Graphic.Window;

public class WindowRegistryManager : IRegistryManager<Window>
{
    private static ConcurrentDictionary<IntPtr, Window> Windows { get; } = new([]);
    private static readonly Lazy<WindowRegistryManager> Registry = new(() => new WindowRegistryManager());

    public static IRegistryManager<Window> Instance() => Registry.Value;

    public void Register(Window window)
    {
        Windows.TryAdd(window.Handle, window);
    }

    public Window? Get(object id)
    {
        if (id is not IntPtr handle) return null;
        return Windows.GetValueOrDefault(handle);
    }

    public Window? Pop(object id)
    {
        if (id is not IntPtr handle) return null;
        Windows.Remove(handle, out var window);
        return window;
    }

    public int Size => Windows.Count;
    public ICollection<Window> Values => Windows.Values.ToHashSet();
}