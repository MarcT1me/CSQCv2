using System.Reflection;

namespace QuantumLauncher;

public abstract class QuantumModule
{
    public Assembly Assembly { get; protected set; } = default;

    public Type? GetAssemblyType(string name)
    {
        return Assembly.GetType(name);
    }

    public FieldInfo? GetAssemblyProp(string type, string name)
    {
        if (GetAssemblyType(type) is { } t)
        {
            return GetAssemblyProp(t, name);
        }
        return null;
    }

    public FieldInfo? GetAssemblyProp(Type type, string name)
    {
        return type.GetField(name, BindingFlags.Static | BindingFlags.Public);
    }

    public void InvokeAssemblyMethod(string type, string name, params object?[] args)
    {
        if (GetAssemblyType(type) is { } t)
            InvokeAssemblyMethod(t, name, args);
    }

    public void InvokeAssemblyMethod(Type type, string name, params object?[] args)
    {
        type.GetMethod(name, BindingFlags.Public | BindingFlags.Static)?.Invoke(null, args);
    }
}