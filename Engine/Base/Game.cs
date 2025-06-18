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

    public override void PostInit()
    {
        MainWindow.Show();
    }

    #region override App cycle

    public sealed override void PreUpdate()
    {
        if (Registries.WindowRegistry.Size == 0)
        {
            Quit();
            return;
        }
        
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

    #endregion
}