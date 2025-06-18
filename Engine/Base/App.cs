namespace Engine.Base;

using Events.QuantumEvents;
using Threading;
using Data.Meta;
using Objects;
using Time;
using Events;
using Failures;
using Logging;
using Extensions;

public abstract class App
    : MetaObject<AppData>,
        IPreparableInstance<AppData>, IReloadData, IExitHandler, IDisposable, // lifetime methods
        IEventful, IUpdatable, IRenderable // loop methods
{
    public Clock Clock { get; }
    public bool Running { get; private set; } = true;
    public static App? Instance { get; private set; }

    public ObjectStatusFlags ObjectStatus => ObjectStatusFlags.All;


    #region Initialization

    protected App() : base(PrepareInstance())
    {
        // ReSharper disable once VirtualMemberCallInConstructor
        PrepareInstance();
        Clock = new(MetaData.ClockMeta);
        QuantumEventHandler.EventHandling += HandleEvent;
        ExitHandling += OnExitHandling;
    }

    public static AppData PrepareInstance()
    {
        return null!;
    }

    #endregion
    
    public virtual void OnReloadData()
    {
        HandleReloadDataEvent();
    }

    #region Mainloop methods

    public static void Mainloop<T>()
        where T : App, new()
    {
        while (Instance == null || Instance.Running)
        {
            Logger.Debug("Mainloop iteration");
            With.Handle(new Catch("Main Mainloop Catch"), _ =>
            {
                Instance = Activator.CreateInstance<T>();
                Instance.Run();
            });

            Instance?.Dispose();
        }
    }

    public void Run()
    {
        PostInit();
        Logger.Info("App started");

        while (Running)
        {
            QuantumEventHandler.HandleEvents();
            QuantumThread.WaitAll();
            if (!Running) break;

            // Update

            PreUpdate();
            QuantumThread.WaitAll();
            if (!Running) break;

            Update();
            QuantumThread.WaitAll();
            if (!Running) break;

            // Render

            PreRender();
            QuantumThread.WaitAll();
            if (!Running) break;

            Render();
            QuantumThread.WaitAll();
            if (!Running) break;

            PostRender();
            QuantumThread.WaitAll();
            if (!Running) break;

            // time updating
            Clock.Tick();
        }

        Logger.Info("App ended");
        HandleExitEvent();
    }

    public virtual void PostInit()
    {
    }

    public abstract void HandleEvent(QuantumEvent e);

    public abstract void PreUpdate();

    public abstract void Update();

    public abstract void PostUpdate();

    public abstract void PreRender();

    public abstract void Render();

    public abstract void PostRender();

    #endregion


    #region Exiting from App

    public virtual void OnExitHandling()
    {
    }

    public virtual void Dispose()
    {
        Quit();
    }

    protected void Quit()
    {
        Running = false;
    }

    #endregion
}