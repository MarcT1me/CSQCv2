#pragma warning disable CS0618 // Type or member is obsolete

namespace Engine.Data.RegistryManagers;

using Collections;
using Asset;
using Decorators;
using Graphic.Window;

public struct Registries
{
    public static readonly IRegistryManager<AssetType> AssetTypeRegistry = AssetTypeRegistryManager.Instance();
    public static readonly IRegistryManager<IDataContainer> DataContainerRegistry = ContainerRegistryManager.Instance();
    public static readonly IRegistryManager<Identifier> IdentifierRegistry = IdentifierRegistryManager.Instance();
    
    public static readonly IRegistryManager<Window> WindowRegistry = WindowRegistryManager.Instance();

    public static readonly IRegistryManager<Type> TypeRegistry = ScanTypeRegistryManager.Instance();
    public static readonly IRegistryManager<QuantumMethodInfo> MethodRegistry = ScanMethodRegistryManager.Instance();
}