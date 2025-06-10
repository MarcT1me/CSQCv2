using System.Collections.Concurrent;
using System.Reflection;

namespace Engine.Data.RegistryManagers;

using Decorators;
using Logging;

internal class DecoratedMethodRegistryManager : IRegistryManager<QuantumMethodInfo>
{
    private static ConcurrentDictionary<string, QuantumMethodInfo> Methods { get; } = new();
    private static readonly Lazy<DecoratedMethodRegistryManager> Registry = 
        new(() => new DecoratedMethodRegistryManager());
    
    public static IRegistryManager<QuantumMethodInfo> Instance() => Registry.Value;
    
    public void Register(QuantumMethodInfo quantumMethod)
    {
        Logger.Info($"Register Quantum Decorator for {quantumMethod.Method.Name}");
        var key = $"{quantumMethod.Method.DeclaringType?.FullName}/{quantumMethod.Method.Name}";
        Methods.TryAdd(key, quantumMethod);
    }

    public QuantumMethodInfo? Get(object id)
    {
        if (id is not MethodInfo method) return null;
        
        var key = $"{method.DeclaringType?.FullName}/{method.Name}";
        return Methods.GetValueOrDefault(key);
    }
}