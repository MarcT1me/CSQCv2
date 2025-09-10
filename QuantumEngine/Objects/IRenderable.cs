namespace QuantumCore.Objects;

using Graphic.Window;

public interface IRenderable : IHasActorStatus
{
    /// <summary>
    /// Подготовка перед отображением (рендер поверхностей и работа над текстом)
    /// </summary>
    void PreRender();

    /// <summary>
    /// Отображение на экран
    /// </summary>
    void Render();

    /// <summary>
    /// Очистка ресурсов после отображения
    /// </summary>
    void PostRender();
}

public interface IWindowRenderable : IHasActorStatus
{
    /// <summary>
    /// Подготовка перед отображением (рендер поверхностей и работа над текстом)
    /// </summary>
    /// <param name="winMeta">Окно для отображения</param>
    void PreRender(WindowData winMeta);

    /// <summary>
    /// Отображение на экран
    /// </summary>
    /// <param name="winMeta">Окно для отображения</param>
    void Render(WindowData winMeta);

    /// <summary>
    /// Очистка ресурсов после отображения
    /// </summary>
    /// <param name="winMeta">Окно для отображения</param>
    void PostRender(WindowData winMeta);
}