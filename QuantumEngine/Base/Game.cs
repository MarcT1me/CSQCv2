// ReSharper disable VirtualMemberCallInConstructor

using QuantumCore.Time;

namespace QuantumCore.Base;

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
    public TWindow MainWindow { get; }

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

    public override void PreUpdate(ClockMeta clockMeta)
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.PreUpdate(clockMeta);
        }
    }

    public override void Update(ClockMeta clockMeta)
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.Update(clockMeta);
        }
    }

    public override void PostUpdate(ClockMeta clockMeta)
    {
        foreach (var window in Registries.WindowRegistry.Values)
        {
            window.PostUpdate(clockMeta);
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
            window.Present();
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