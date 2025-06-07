// ReSharper disable CollectionNeverUpdated.Local

using System.Diagnostics;
using System.Reflection;

namespace Engine.Extensions.Tracer;

using Data.RegistryManagers;

/// <summary>
/// Аттрибут для добавления классов и методов в список типов движка (можно будет использовать в любой момент)
/// </summary>
/// <param name="scanType">Тип, указывающий сканеру область для сохранения</param>
[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method)]
public class QTraceAttribute(ScanTypes scanType) : Attribute
{
    /// <summary>
    /// Метод для регистрирования классов
    /// </summary>
    public void ScanHandling(Type @class)
    {
        Debug.Assert(
            scanType == ScanTypes.Scan,
            "traceType does not match the Scan"
        );

        Registries.TypeRegistry.Register(@class);
    }

    /// <summary>
    /// Метод для регистрирования методов классов
    /// </summary>
    public void ScanHandling(Type @class, MethodInfo method)
    {
        Debug.Assert(
            scanType is ScanTypes.Callback or ScanTypes.Bind,
            "traceType does not match the Callback or Bind"
        );

        if (Registries.TypeRegistry.Get(@class.Name) == null)
            Registries.TypeRegistry.Register(@class);

        Registries.MethodRegistry.Register(method);
    }
}