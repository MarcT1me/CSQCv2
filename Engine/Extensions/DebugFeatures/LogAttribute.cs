using System.Reflection;

namespace Engine.Extensions.DebugFeatures;

using Decorators;
using Logging;
using Configuration;

/// <summary>
/// Декорирующий аттрибут, выводящий сообщение при вызове
/// </summary>
/// <param name="logMessage">Сообщение лога</param>
[AttributeUsage(AttributeTargets.Method | AttributeTargets.Event | AttributeTargets.Constructor)]
public class LogAttribute(string logMessage, bool debugOnly = false) : Attribute, IQuantumDecorator
{
    public object? Intercept(object? target, MethodInfo targetMethod, object?[]? args, Func<object?> proceed)
    {
        if (debugOnly)
        {
            if (BaseConfig.DebugMode)
            {
                Logger.Info(logMessage);
            }
        }
        else
        {
            Logger.Info(logMessage);
        }

        return proceed();
    }
}