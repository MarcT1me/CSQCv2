using System.Diagnostics;
using System.Reflection;

namespace Engine.Data.Tracer;

using Decorators;
using RegistryManagers;

/// <summary>
/// Quantum Tracer - система трассировки и автоматического применения декораторов
/// </summary>
[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method)]
public class QTraceAttribute(ScanTypes scanType) : Attribute
{
    private static readonly Dictionary<Type, bool> DecoratedTypeCache = new();
    private static readonly HashSet<Type> _processedTypes = new();

    public static void HandleAssembly()
    {
        var types = EngineCore.AppLibAssembly.GetTypes();

        // Сначала сканируем все типы
        foreach (var type in types)
        {
            HandleTypeScanning(type);
        }

        // Затем обрабатываем декораторы для всех найденных типов
        ProcessDecoratedTypes();
    }

    private static void HandleTypeScanning(Type type)
    {
        HandleClasses(type);

        // Сканируем методы
        foreach (var method in type.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static))
        {
            HandleMethods(type, method);
        }
    }

    private static void HandleClasses(Type type)
    {
        var classAttributes = type.GetCustomAttributes(false);
        foreach (var attr in classAttributes.OfType<QTraceAttribute>())
        {
            attr.ScanHandling(type);
        }

        // Проверяем, есть ли декорированные методы в классе
        if (HasDecoratedMethods(type))
        {
            DecoratedTypeCache[type] = true;
            QuantumTracer.RegisterDecoratedType(type);
        }
    }

    private static void HandleMethods(Type type, MethodInfo method)
    {
        var methodAttributes = method.GetCustomAttributes(false);

        foreach (var attr in methodAttributes.OfType<QTraceAttribute>())
        {
            attr.ScanHandling(type, method);
        }

        // Регистрируем методы с декораторами
        if (methodAttributes.OfType<IDecoratorAttribute>().Any())
        {
            QuantumTracer.RegisterDecoratedMethod(type, method);
        }
    }

    private static void ProcessDecoratedTypes()
    {
        foreach (var type in DecoratedTypeCache.Keys)
        {
            if (!_processedTypes.Contains(type))
            {
                QuantumTracer.ProcessTypeForDecorators(type);
                _processedTypes.Add(type);
            }
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

    private static bool HasDecoratedMethods(Type type)
    {
        return type.GetMethods(BindingFlags.Public | BindingFlags.Instance)
            .Any(m => m.GetCustomAttributes().OfType<IDecoratorAttribute>().Any());
    }
}