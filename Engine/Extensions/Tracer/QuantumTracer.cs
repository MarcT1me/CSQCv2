using System.Reflection;

namespace Engine.Extensions.Tracer;

using Data.RegistryManagers;
using Decorators;
using Logging;

/// <summary>
/// Quantum Tracer - система трассировки и автоматического применения декораторов
/// </summary>
public static class QuantumTracer
{
    /// <summary>
    /// Метод сканирования домена AppLib на наличие аттрибутов
    /// </summary>
    [Obsolete("It is used only in the engine, not in game logic.", true)]
    public static void HandleAssembly(Assembly?[] assemblies)
    {
        foreach (var assembly in assemblies)
        {
            if (assembly is null) continue;

            Logger.Info($"Scanning assembly: {assembly.FullName}");
            ProcessAssembly(assembly);
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

    public static void ProcessAssembly(Assembly assembly)
    {
        var types = assembly.GetTypes();

        foreach (var type in types)
        {
            ProcessType(type);

            // Сканируем методы
            foreach (var method in type.GetMethods(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance))
            {
                ProcessMethod(type, method);
            }
        }
    }

    public static void ProcessType(Type type)
    {
        var classAttributes = type.GetCustomAttributes(false);
        foreach (var attr in classAttributes.OfType<QTraceAttribute>())
        {
            attr.ScanHandling(type);
        }
    }

    public static void ProcessMethod(Type type, MethodInfo methodInfo)
    {
        var methodAttributes = methodInfo.GetCustomAttributes(false);

        var scanAttr = methodAttributes.OfType<QTraceAttribute>().ToHashSet();
        if (scanAttr.Count == 1)
        {
            scanAttr.First().ScanHandling(type, methodInfo);
        }

        var decorators = methodAttributes.OfType<QuantumDecoratorAttribute>().ToList();
        if (decorators.Count > 0)
        {
            Logger.Debug($"Found {decorators.Count} decorators for method {methodInfo.Name}");
            
            var qmInfo = new QuantumMethodInfo(methodInfo, decorators);
            
            // Проверяем, есть ли уже такой метод в реестре
            var existingMethod = Registries.MethodRegistry.Get(methodInfo);
            if (existingMethod != null)
            {
                Logger.Debug($"Method {methodInfo.Name} already registered, updating decorators");
                // Объединяем декораторы если нужно
                var allDecorators = existingMethod.Decorators.Concat(decorators).Distinct().ToList();
                qmInfo = new QuantumMethodInfo(methodInfo, allDecorators);
            }
            
            Registries.MethodRegistry.Register(qmInfo);
        
            // Применяем декораторы через Harmony
            QuantumIlRewriter.ApplyDecorators(qmInfo);
        }
    }
}