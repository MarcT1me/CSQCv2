namespace Engine.Data.RegistryManagers;

using Graphic.Window;

public struct Registries
{
    public static readonly IRegistryManager<Window> WindowRegistry = WindowRegistryManager.Instance();
}