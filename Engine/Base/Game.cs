// ReSharper disable VirtualMemberCallInConstructor

namespace Engine.Base;

using Graphic.Window;
using Data.RegistryManagers;

public abstract class Game<T> : App where T : Window
{
    protected T MainWindow { get; }

    protected Game()
    {
        MainWindow = CreateMainWindow();
    }

    protected abstract T CreateMainWindow();

    #region override App cycle

    public sealed override void PreUpdate()
    {
        if (Registries.WindowRegistry.Size == 0) Quit();
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.PreUpdate();
        }
    }

    public sealed override void Update()
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.Update();
        }
    }

    public sealed override void PostUpdate()
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            PostUpdate();
        }
    }

    public sealed override void PreRender()
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.PreRender();
        }
    }

    public sealed override void Render()
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.Render();
        }
    }

    public sealed override void PostRender()
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            PostRender();
        }
    }

    #endregion
}