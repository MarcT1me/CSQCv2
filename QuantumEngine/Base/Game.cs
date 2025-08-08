// ReSharper disable VirtualMemberCallInConstructor

namespace Engine.Base;

using Graphic.Window;
using Data.RegistryManagers;

/// <summary>
/// Базовый класс игры
/// </summary>
/// <typeparam name="TData">Класс данных игры</typeparam>
/// <typeparam name="TWindow">Класс главного окна</typeparam>
public abstract class Game<TData, TWindow> : App<TData> 
    where TData : AppData
    where TWindow : Window
{
    protected TWindow MainWindow { get; }

    protected Game()
    {
        MainWindow = CreateMainWindow();
    }

    protected abstract TWindow CreateMainWindow();

    public override void Prepare()
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.Prepare();
        }
    }

    #region override App cycle

    public override void PreUpdate()
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.PreUpdate();
        }
    }

    public override void Update()
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.Update();
        }
    }

    public override void PostUpdate()
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.PostUpdate();
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
            window.PostRender();
        }
    }

    public override void OnExitHandling()
    {
        base.OnExitHandling();
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.Dispose();
        }
    }

    #endregion
}