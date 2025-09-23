using MirageAPI;
using MirageAPI.DirectX;
using OpenTK.Mathematics;
using QuantumCore.Events.QuantumEvents.Window;
using QuantumCore.Time;

// engine sub-systems

namespace QuantumCore.Graphic.Window;

using Events;
using Events.QuantumEvents;
using Objects;
using Data.Meta;
using Logging;
using Data.RegistryManagers;

public class Window
    : MetaObject<WindowData>, IPreparableInstance, IPreparable,
        IEventful, IUpdatable, IRenderable
{
    public static Window? ActiveWindow { get; protected set; }

    public ObjectStatusFlags ObjectStatus => MetaData.Status;

    public IntPtr Handle => NativeWindow.Handle;
    public readonly MirageAPI.Window NativeWindow;
    public readonly Window? ParentWindow;

    public Window(
        WinData winData,
        GlData? glData = null,
        string? name = null,
        Window? parent = null,
        DisplayInfo? display = null
    ) : base(new WindowData(winData, glData ?? new GlData(), name))
    {
        Logger.Info(
            $"Creating window '{MetaData.Identifier}'\n" +
            $"Position: {winData.Position}\n" +
            $"Size: {winData.Size}\n"
        );

        ParentWindow = parent;

        NativeWindow = new MirageAPI.Window(
            WindowClass.Default,
            WindowType.Overlapped,
            MetaData.Identifier.GetNameAnyway(),
            new Rect
            {
                X = MetaData.WinData.Position.X,
                Y = MetaData.WinData.Position.Y,
                Width = MetaData.WinData.Size.X,
                Height = MetaData.WinData.Size.Y
            },
            MetaData.WinData.Fullscreen,
            MetaData.WinData.WindowStyle,
            parent?.NativeWindow, display,
            // ReSharper disable once VirtualMemberCallInConstructor
            CreateWindowContextConfig()
        );

        // ReSharper disable once VirtualMemberCallInConstructor
        PrepareInstance();

        Registries.WindowRegistry.Register(this);
    }

    public virtual void PrepareInstance()
    {
        QEventSystem.RegisterWindow(NativeWindow);
        QEventSystem.EventHandling += HandleEvent;

        Input.Mouse.Mouse.RegisterWindow(this);
        Input.Keyboard.Keyboard.RegisterWindow(this);
    }

    protected virtual DX12ContextConfig CreateWindowContextConfig()
    {
        var windowContextConfig = new DX12ContextConfig
        (
            Id,
            
            Viewport: MetaData.GlData.Viewport,
            Near: MetaData.GlData.ClipPlanes.X,
            Far: MetaData.GlData.ClipPlanes.Y,

            Resolution: MetaData.WinData.Resolution,

            BufferCount: MetaData.GlData.MaxFramesInFlight,
            Format: MetaData.GlData.Format,

            SampleCount: MetaData.GlData.NumberOfSamples,
            SwapQuality: MetaData.GlData.SwapQuality,

            SwapEffect: MetaData.GlData.SwapEffect,
            VSyncInterval: MetaData.WinData.VSyncInterval,
            
            null
        );

        return windowContextConfig;
    }

    public virtual void Prepare()
    {
        Establish();
    }

    protected void Establish() => NativeWindow.Establish();

    public string Title
    {
        get => NativeWindow.Title;
        set => NativeWindow.Title = value;
    }

    public Vector2i Size
    {
        get => MetaData.WinData.Size;
        set => NativeWindow.SetSize(MetaData.WinData.Size = value);
    }

    public float ResolutionScaling
    {
        get => MetaData.WinData.ResolutionScaling;
        set => MetaData.WinData.ResolutionScaling = value;
    }

    public Vector2i Position
    {
        get => MetaData.WinData.Position;
        set => NativeWindow.SetPosition(MetaData.WinData.Position = value);
    }

    public Rect Rect
    {
        get => NativeWindow.CurrentRect;
        set => NativeWindow.CurrentRect = value;
    }

    public bool IsFullscreen
    {
        get => MetaData.WinData.Fullscreen;
        set => MetaData.WinData.Fullscreen = NativeWindow.IsFullscreen = value;
    }

    public WindowStyleInfo Style => MetaData.WinData.WindowStyle;

    public DisplayInfo Display
    {
        get => NativeWindow.Display;
        set => NativeWindow.Display = value;
    }

    public uint VSyncInterval
    {
        get => MetaData.WinData.VSyncInterval;
        set => MetaData.WinData.VSyncInterval = NativeWindow.DXContext.VSync = value;
    }

    public Rect Viewport
    {
        get => MetaData.GlData.Viewport;
        set => NativeWindow.DXContext.SetViewport(MetaData.GlData.Viewport = value);
    }

    public Vector2 ClipPlanes
    {
        get => MetaData.GlData.ClipPlanes;
        set => NativeWindow.DXContext.SetClipPlanes(MetaData.GlData.ClipPlanes = value);
    }

    public bool IsEnabled => NativeWindow.IsEnabled;

    public bool IsVisible
    {
        get => NativeWindow.IsVisible;
        set => NativeWindow.IsVisible = value;
    }

    public bool IsMaximized
    {
        get => NativeWindow.IsMaximized;
        set => NativeWindow.IsMaximized = value;
    }

    public bool IsIcon => NativeWindow.IsIcon;

    protected void Restore() => NativeWindow.Restore();
    protected void BringToFront() => NativeWindow.BringToFront();
    protected void Flash(uint count, uint timeout) => NativeWindow.Flash(count, timeout);
    protected void HideToTray() => NativeWindow.HideToTray();
    protected void ShowFromTray() => NativeWindow.ShowFromTray();
    protected void Update() => NativeWindow.Update();

    protected void BeginFrame() => NativeWindow.BeginFrame();
    protected void Clear(Color4 clearColor) => NativeWindow.Clear(clearColor);
    protected void EndFrame() => NativeWindow.EndFrame();
    public void Present() => NativeWindow.Present();

    public virtual void HandleEvent(QuantumEvent e)
    {
        switch (e)
        {
            case { Type: EventType.WindowClose }:
                Dispose();
                break;
            case { Type: EventType.WindowFocusGained }:
                ActiveWindow = this;
                break;
            case { Type: EventType.WindowFocusLost } when ActiveWindow == this:
                ActiveWindow = null;
                break;
            case WinResizeEvent winResize:
                MetaData.WinData.Size = winResize.Size;
                NativeWindow.DXContext.SetResolution(MetaData.WinData.Resolution);
                break;
        }
    }

    public virtual void PreUpdate(ClockMeta clockMeta)
    {
    }

    public virtual void Update(ClockMeta clockMeta)
    {
    }

    public virtual void PostUpdate(ClockMeta clockMeta)
    {
    }

    public virtual void PreRender()
    {
        BeginFrame();
    }

    public virtual void Render()
    {
        Clear(MetaData.GlData.ClearColor);
    }

    public virtual void PostRender()
    {
        EndFrame();
    }

    public virtual void Dispose()
    {
        Registries.WindowRegistry.Pop(Handle);

        QEventSystem.UnregisterWindow(NativeWindow);
        QEventSystem.EventHandling -= HandleEvent;

        Input.Mouse.Mouse.UnregisterWindow(this);
        Input.Keyboard.Keyboard.UnregisterWindow(this);

        NativeWindow.Dispose();

        Logger.Separator();
        Logger.Success($"Window '{Id}' disposed");
        GC.SuppressFinalize(this);
    }

    ~Window() => Dispose();
}