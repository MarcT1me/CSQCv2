// ReSharper disable VirtualMemberCallInConstructor

namespace Engine.Base;

using Threading;
using Data.Meta;
using Objects;
using Time;
using Events;
using Events.QuantumEvents;
using Failures;
using Logging;
using Extensions;

public abstract class App<TData>
    : MetaObject<TData>,
        IPreparableInstance<TData>, IReloadData, IExitHandler, // lifetime methods
        IEventful, IUpdatable, IRenderable // loop methods
    where TData : AppData
{
    public Clock Clock { get; }

    // ReSharper disable once StaticMemberInGenericType
    private static bool _running = true;
    public static App<TData>? Instance { get; private set; }
    public ObjectStatusFlags ObjectStatus => MetaData.ObjectStatus;

    #region Initialization

    protected App() : base(null!)
    {
        MetaData = PrepareInstance();
        Clock = new(MetaData.Tps, MetaData.SpeedRoster);
        QEventSystem.EventHandling += HandleEvent;
        ExitHandling += OnExitHandling;
    }

    public abstract TData PrepareInstance();

    public virtual void PostInit()
    {
    }

    #endregion

    public virtual void OnReloadData(MetaData data)
    {
        HandleReloadDataEvent(data);
    }

    #region Mainloop methods

    public static void Mainloop(Type appType)
    {
        while (_running || (Instance is not null && Instance.ObjectStatus.HasFlag(ObjectStatusFlags.Active)))
        {
            var cth = new Catch("Mainloop iteration Catch");

            With.Handle(cth, _ =>
            {
                Logger.Separator();
                Logger.Debug("Mainloop Iteration");

                using (Instance = Activator.CreateInstance(appType) as App<TData>)
                {
                    if (Instance == null) throw new Exception("App Instance could not be created");

                    Instance.Run();
                }
            });

            _running = cth.MetaData.Failures.Count != 0;
        }
    }

    public void Run()
    {
        Instance = this;
        PostInit();

        Logger.Separator();
        Logger.Info("App Run started");

        bool IsActive() => ObjectStatus.HasFlag(ObjectStatusFlags.Active);
        
        Clock.StartClock();
        while (IsActive())
        {
            QEventSystem.PollEvents();
            QuantumThread.WaitAll();
            if (!IsActive()) break;

            // Update

            PreUpdate();
            QuantumThread.WaitAll();
            if (!IsActive()) break;

            Update();
            QuantumThread.WaitAll();
            if (!IsActive()) break;

            PostUpdate();
            QuantumThread.WaitAll();
            if (!IsActive()) break;
            // Render

            PreRender();
            QuantumThread.WaitAll();
            if (!IsActive()) break;

            Render();
            QuantumThread.WaitAll();
            if (!IsActive()) break;

            PostRender();
            QuantumThread.WaitAll();
            if (!IsActive()) break;

            // time updating
            Clock.Tick();
        }

        Logger.Separator();
        Logger.Info("App Run ended");
        HandleExitEvent();
    }

    public virtual void HandleEvent(QuantumEvent e)
    {
        if (e is { Type: EventType.Quit })
        {
            Quit();
        }
    }

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
        Logger.Info("App exiting handling");
    }

    public virtual void Dispose()
    {
        Quit();
        Instance = null;

        // QuantumEventHandler.EventHandling -= HandleEvent;
        ExitHandling -= OnExitHandling;

        Logger.Info("App disposed");
        Logger.Separator();

        GC.SuppressFinalize(this);
    }

    protected void Quit()
    {
        Logger.Separator();
        
        if (!MetaData.ObjectStatus.HasFlag(ObjectStatusFlags.Active)) return;
        MetaData.ObjectStatus &= ~ObjectStatusFlags.Active;

        Logger.Info("App quit");
    }

    #endregion
}