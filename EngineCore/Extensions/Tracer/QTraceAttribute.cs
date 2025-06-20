// ReSharper disable CollectionNeverUpdated.Local

using System.Diagnostics;
using System.Reflection;

namespace Engine.Extensions.Tracer;

using Data.RegistryManagers;
using Logging;
using Decorators;

/// <summary>
/// Аттрибут для добавления классов и методов в список типов движка (можно будет использовать в любой момент)
/// </summary>
/// <param name="scanType">Тип, указывающий сканеру область для сохранения</param>
[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method)]
public class QTraceAttribute(ScanTypes scanType) : QuantumAttribute
{
    /// <summary>
    /// Метод для регистрирования классов
    /// </summary>
    public void ScanHandling(Type @class)
    {
        Debug.Assert(scanType == ScanTypes.Scan, "traceType does not match the Scan");

        Logger.Info($"Add new scanned class [{@class.Name}]");

        CoreRegistries.TypeRegistry.Register(@class);
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

        if (CoreRegistries.TypeRegistry.Get(@class.Name) == null)
            ScanHandling(@class);

        Logger.Info($"Add new scanned method [{method.Name}] -> {scanType}");

        CoreRegistries.MethodRegistry.Register(new QuantumMethodInfo(method));
    }
}