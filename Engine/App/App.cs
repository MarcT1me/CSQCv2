using Engine.Events.QuantumEvents;
using Engine.Threading;

namespace Engine.App;

using Data.Meta;
using Objects;
using Time;
using Events;
using Failures;
using Logging;

public abstract class App
    : MetaObject<AppData>,
        IPreparableInstance, IExitHandler, IDisposable, // lifetime methods
        IEventful, IUpdatable, IRenderable // loop methods
{
    public Clock Clock { get; }
    public bool Running { get; private set; } = true;
    public static App? Instance { get; private set; }

    public ObjectStatusFlags ObjectStatus => ObjectStatusFlags.All;


    #region Initialization

    protected App(AppData appData) : base(appData)
    {
        // ReSharper disable once VirtualMemberCallInConstructor
        PrepareInstance();
        Clock = new(appData.ClockMeta);
        ExitHandling += OnExitHandling;
    }

    public virtual void PrepareInstance()
    {
    }

    public abstract object CreateWinData();

    public abstract object CreateGlData();

    #endregion


    #region Mainloop methods

    public static void Mainloop<T>()
        where T : App, new()
    {
        while (Instance == null || Instance.Running)
        {
            Logger.Debug("Mainloop iteration");
            using (new Catch())
            {
                Instance = Activator.CreateInstance<T>();
                Instance.Run();
            }

            Instance.Dispose();
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

            PreUpdate();
            QuantumThread.WaitAll();
            Update();
            QuantumThread.WaitAll();

            PreRender();
            QuantumThread.WaitAll();
            Render();
            QuantumThread.WaitAll();
            PostRender();
            QuantumThread.WaitAll();

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