using System.Reflection;

namespace Engine.Extensions.DebugFeatures;

using Decorators;
using Logging;
using Configuration;

/// <summary>
/// Декорирующий аттрибут, выводящий сообщение при вызове
/// </summary>
/// <param name="logMessage">Сообщение лога</param>
[Obsolete("BROKEN", true)]
[AttributeUsage(AttributeTargets.Method)]
public class LogAttribute(string logMessage, bool debugOnly = false) : QuantumDecoratorAttribute
{
    public override object? Intercept(object? target, MethodInfo targetMethod, object?[]? args, Func<object?> proceed)
    {
        if (debugOnly && !BaseConfig.DebugMode)
            return proceed();

        Logger.Separator();
        Logger.Info($"\e[36m{targetMethod.DeclaringType?.Name}.{targetMethod.Name}\e[0m \e[31m-\e[0m {logMessage}");

        // Логируем аргументы если они есть
        if (args != null && args.Length > 0)
        {
            Logger.Info($"Arguments: [{string.Join(", ", args.Select(a => a?.ToString() ?? "null"))}]");
        }

        Logger.Separator();

        // Вызываем оригинальный метод
        var result = proceed();

        // Логируем результат если он есть
        if (targetMethod.ReturnType != typeof(void) && result != null)
        {
            Logger.Info($"Returned: {result}");
        }

        return result;
    }
}