using System.Diagnostics;
using System.Reflection;

namespace Engine.Data.Tracer;

using Decorators;
using RegistryManagers;

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method)]
public class QTraceAttribute(ScanTypes scanType) : Attribute
{
    public static void HandleAssembly()
    {
        var types = EngineCore.AppLibAssembly.GetTypes();

        foreach (var type in types)
        {
            HandleClasses(type);

            // handle method attributes
            foreach (var method in type.GetMethods())
            {
                HandleMethods(type, method);
            }
        }
    }

    private static void HandleClasses(Type type)
    {
        var classAttributes = type.GetCustomAttributes(false);
        foreach (var attr in classAttributes.OfType<QTraceAttribute>())
        {
            attr.ScanHandling(type);
        }
    }

    private static void HandleMethods(Type type, MethodInfo method)
    {
        var methodAttributes = method.GetCustomAttributes(false);

        foreach (var attr in methodAttributes.OfType<QTraceAttribute>())
        {
            attr.ScanHandling(type, method);
        }
        foreach (var _ in methodAttributes.OfType<IDecoratorAttribute>())
        {
            HandleMethodProxy(type, method);
        }
    }

    private void ScanHandling(Type @class)
    {
        Debug.Assert(
            scanType == ScanTypes.Scan,
            "traceType does not match the Scan"
        );

        Registries.TypeRegistry.Register(@class);
    }

    private void ScanHandling(Type @class, MethodInfo method)
    {
        Debug.Assert(
            scanType is ScanTypes.Callback or ScanTypes.Bind,
            "traceType does not match the Callback or Bind"
        );

        if (Registries.TypeRegistry.Get(@class.Name) == null)
            Registries.TypeRegistry.Register(@class);

        Registries.MethodRegistry.Register(method);
    }

    private static void HandleMethodProxy(Type @class, MethodInfo method)
    {
    }
}