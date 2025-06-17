using System.Reflection;
using Engine.Logging;
using HarmonyLib;

namespace Engine.Extensions.Tracer;

using Decorators;
using Data.RegistryManagers;

public static class QuantumIlRewriter
{
    private static readonly Dictionary<MethodInfo, QuantumMethodInfo> MethodMap = new();
    private static readonly Harmony Harmony = new("quantum.decorators");

    public static void ApplyDecorators(QuantumMethodInfo methodInfo)
    {
        // Сохраняем маппинг метода к его декораторам
        MethodMap[methodInfo.Method] = methodInfo;

        Logger.Debug($"Applying decorators to method: {methodInfo.Method.Name}");
        Logger.Debug($"Decorators count: {methodInfo.Decorators.Count()}");

        try
        {
            // Применяем патч с помощью Harmony
            Harmony.Patch(
                original: methodInfo.Method,
                prefix: new HarmonyMethod(typeof(QuantumIlRewriter), nameof(Prefix))
            );
            
            Logger.Debug($"Successfully patched method: {methodInfo.Method.Name}");
        }
        catch (Exception ex)
        {
            Logger.Error($"Failed to patch method {methodInfo.Method.Name}: {ex.Message}");
        }
    }

    // Флаг для предотвращения рекурсии
    private static readonly ThreadLocal<HashSet<MethodInfo>> ExecutingMethods = 
        new(() => new HashSet<MethodInfo>());

    private static bool Prefix(
        object? __instance,
        MethodBase __originalMethod,
        object?[]? __args,
        ref object? __result)
    {
        var methodInfo = __originalMethod as MethodInfo;
        if (methodInfo == null) 
        {
            return true;
        }

        // Проверяем, не выполняется ли уже этот метод (предотвращение рекурсии)
        if (ExecutingMethods.Value!.Contains(methodInfo))
        {
            return true; // Выполняем оригинальный метод без декораторов
        }

        // Ищем зарегистрированные декораторы для метода
        if (!MethodMap.TryGetValue(methodInfo, out var qmInfo) || !qmInfo.Decorators.Any()) 
        {
            return true;
        }

        try
        {
            // Отмечаем, что метод выполняется
            ExecutingMethods.Value.Add(methodInfo);

            // Строим цепочку вызовов - оригинальный метод будет вызван через return true
            Func<object?> proceed = () =>
            {
                try
                {
                    // Временно убираем метод из выполняющихся для вызова оригинала
                    ExecutingMethods.Value.Remove(methodInfo);
                    return methodInfo.Invoke(__instance, __args);
                }
                finally
                {
                    // Возвращаем обратно
                    ExecutingMethods.Value.Add(methodInfo);
                }
            };

            // Применяем декораторы в обратном порядке для правильной цепочки
            var decorators = qmInfo.Decorators.Reverse().ToList();
            foreach (var decorator in decorators)
            {
                var currentProceed = proceed;
                proceed = () => decorator.Intercept(__instance, methodInfo, __args, currentProceed);
            }

            var result = proceed();

            if (methodInfo.ReturnType != typeof(void))
            {
                __result = result;
            }

            return false; // Пропускаем оригинальный метод
        }
        catch (Exception ex)
        {
            Logger.Error($"Error in decorator chain: {ex.Message}");
            return true; // Если ошибка, выполняем оригинальный метод
        }
        finally
        {
            // Убираем метод из выполняющихся
            ExecutingMethods.Value.Remove(methodInfo);
        }
    }
}