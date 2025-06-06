namespace Engine.Extensions;

using Decorators;

public static class ObjectExtensions
{
    /// <summary>
    /// Создает объект с автоматическим применением декораторов
    /// </summary>
    public static T WithDecorators<T>(this T instance) where T : class
    {
        return DecoratorFactory.CreateIfNeeded(instance);
    }
}