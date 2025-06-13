using System.Reflection;

namespace Engine.Decorators;

/// <summary>
/// Базовый интерфейс для всех декорирующих аттрибутов
/// </summary>
public abstract class QuantumDecoratorAttribute : QuantumAttribute
{
    /// <summary>
    /// Перехватывает вызов метода
    /// </summary>
    /// <param name="target">Целевой объект</param>
    /// <param name="targetMethod">Метод для вызова</param>
    /// <param name="args">Аргументы метода</param>
    /// <param name="proceed">Функция для продолжения выполнения оригинального метода</param>
    /// <returns>Результат выполнения</returns>
    public abstract object? Intercept(
        object? target,
        MethodInfo targetMethod,
        object?[]? args,
        Func<object?> proceed
    );
}