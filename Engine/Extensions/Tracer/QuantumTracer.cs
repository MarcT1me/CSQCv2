using System.Reflection;

namespace Engine.Extensions.Tracer;

using Data.RegistryManagers;
using Decorators;

/// <summary>
/// Quantum Tracer - система трассировки и автоматического применения декораторов
/// </summary>
public static class QuantumTracer
{
    /// <summary>
    /// Метод сканирования домена AppLib на наличие аттрибутов
    /// </summary>
    [Obsolete("It is used only in the engine, not in game logic.", true)]
    public static void HandleAssembly()
    {
        var assembly = GetScanned("Engine", ScanTypes.Assembly) as Assembly;

        // Сначала сканируем все типы движка
        var types = assembly?.GetTypes();
        if (types == null) return;
        foreach (var type in types)
        {
            HandleTypeScanning(type);
        }

        // Потом игры
        types = EngineCore.AppLibAssembly.GetTypes();
        foreach (var type in types)
        {
            HandleTypeScanning(type);
        }
    }

    /// <summary>
    /// Выдаёт объекты, обрабатываемые Quantum Trace
    /// </summary>
    /// <param name="id">Идентификатор объекта</param>
    /// <param name="scanType">Тип сканированного объекта</param>
    /// <returns>Объект, если нашёлся</returns>
    public static object? GetScanned(object id, ScanTypes scanType)
    {
        return (id, scanType) switch
        {
            (_, ScanTypes.Scan) => Registries.TypeRegistry.Get(id),
            (_, ScanTypes.Callback) => Registries.MethodRegistry.Get(id),
            (string name, ScanTypes.Assembly) => AppDomain.CurrentDomain.GetAssemblies()
                .FirstOrDefault(a => a.GetName().Name == name),
            _ => null
        };
    }

    private static void HandleTypeScanning(Type type)
    {
        HandleClass(type);

        // Сканируем методы
        foreach (var method in type.GetMethods(
                     BindingFlags.Static | BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic)
                )
        {
            HandleMethod(type, method);
        }
    }

    private static void HandleClass(Type type)
    {
        var classAttributes = type.GetCustomAttributes(false);
        foreach (var attr in classAttributes.OfType<QTraceAttribute>())
        {
            attr.ScanHandling(type);
        }

        RuntimeDecorator.SaveClassDecorators(type);
    }

    private static void HandleMethod(Type type, MethodInfo method)
    {
        var methodAttributes = method.GetCustomAttributes(false);

        foreach (var attr in methodAttributes.OfType<QTraceAttribute>())
        {
            attr.ScanHandling(type, method);
        }
    }
}