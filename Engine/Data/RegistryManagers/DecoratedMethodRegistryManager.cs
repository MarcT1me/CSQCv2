using System.Collections.Concurrent;
using System.Reflection;

namespace Engine.Data.RegistryManagers;

using Decorators;

public class DecoratedMethodRegistryManager : IRegistryManager<DecoratedMethodInfo>
{
    private static ConcurrentDictionary<string, DecoratedMethodInfo> Methods { get; } = new();
    private static readonly Lazy<DecoratedMethodRegistryManager> Registry = 
        new(() => new DecoratedMethodRegistryManager());
    
    public static IRegistryManager<DecoratedMethodInfo> Instance() => Registry.Value;
    
    public void Register(DecoratedMethodInfo decoratedMethod)
    {
        var key = $"{decoratedMethod.Method.DeclaringType?.FullName}/{decoratedMethod.Method.Name}";
        Methods[key] = decoratedMethod;
    }

    public DecoratedMethodInfo? Get(object id)
    {
        if (id is not MethodInfo method) return null;
        
        var key = $"{method.DeclaringType?.FullName}/{method.Name}";
        return Methods.GetValueOrDefault(key);
    }
}