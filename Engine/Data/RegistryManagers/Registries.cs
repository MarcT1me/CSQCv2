#pragma warning disable CS0618 // Type or member is obsolete

using System.Reflection;

namespace Engine.Data.RegistryManagers;

using Arrays;
using Asset;

public struct Registries
{
    public static readonly IRegistryManager<Type> TypeRegistry = ScanTypeRegistryManager.Instance();
    public static readonly IRegistryManager<MethodInfo> MethodRegistry = ScanMethodRegistryManager.Instance();
    public static readonly IRegistryManager<IDataContainer> DataContainerRegistry = ContainerRegistryManager.Instance();
    public static readonly IRegistryManager<Identifier> IdentifierRegistry = IdentifierRegistryManager.Instance();
    public static readonly IRegistryManager<AssetType> AssetTypeRegistry = AssetTypeRegistryManager.Instance();
}