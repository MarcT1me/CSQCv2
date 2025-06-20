namespace Engine.Objects;

public interface IRenderable : IHasActorStatus
{
    public bool IsVisible() => IsActive() || ObjectStatus.HasFlag(ObjectStatusFlags.Visible);

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