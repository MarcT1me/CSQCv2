using System.Reflection;
using System.Runtime.CompilerServices;

namespace Engine.Decorators;

public static class QuantumInvoker
{
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static object? InvokeMethod(object instance, MethodInfo method, object?[]? args = null)
    {
        return RuntimeDecorator.InvokeDecoratedMethod(instance, method, args);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static object? InvokeMethod(MethodInfo method, object?[]? args = null)
    {
        return RuntimeDecorator.InvokeDecoratedMethod(null, method, args);
    }
}