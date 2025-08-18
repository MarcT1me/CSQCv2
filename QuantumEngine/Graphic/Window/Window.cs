using Engine.Events.QuantumEvents.Window;
using Engine.Time;
using MirageAPI;
using MirageAPI.DirectX;
using OpenTK.Mathematics;

// engine sub-systems

namespace Engine.Graphic.Window;

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
    protected readonly MirageAPI.Window NativeWindow;
    public IntPtr Handle => NativeWindow.Handle;

    public Window? ActiveWindow { get; protected set; }
    public Window? ParentWindow { get; }
    public ObjectStatusFlags ObjectStatus => MetaData.Status;

    public Window(
        WinData winData,
        GlData? glData = null,
        string? name = null,
        Window? parent = null,
        DisplayInfo? display = null,
        IconInfo? icon = null,
        CursorInfo? cursor = null
    ) : base(new WindowData(winData, glData ?? new GlData(), name))
    {
        Logger.Info(
            $"Creating window '{MetaData.Identifier}'\n" +
            $"Position: {winData.Position}\n" +
            $"Size: {winData.Size}\n"
        );

        ParentWindow = parent;

        NativeWindow = new MirageAPI.Window(
            WindowType.Overlapped,
            new SimpleRect
            {
                X = MetaData.WinData.Position.X,
                Y = MetaData.WinData.Position.Y,
                Width = MetaData.WinData.Size.X,
                Height = MetaData.WinData.Size.Y
            },
            MetaData.Identifier.GetNameAnyway(),
            MetaData.WinData.Opacity,
            MetaData.WinData.Fullscreen,
            parent?.NativeWindow, display, icon, cursor,
            CreateDefaultWindowContextConfig()
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

    protected DX12ContextConfig CreateDefaultWindowContextConfig()
    {
        var windowContextConfig = new DX12ContextConfig
        {
            Viewport = MetaData.GlData.Viewport,
            Near = MetaData.GlData.ClipPlanes.X,
            Far = MetaData.GlData.ClipPlanes.Y,

            Resolution = MetaData.WinData.Resolution,

            BufferCount = MetaData.GlData.MaxFramesInFlight,
            Format = MetaData.GlData.Format,

            SampleCount = MetaData.GlData.NumberOfSamples,
            SwapQuality = MetaData.GlData.SwapQuality,

            SwapEffect = MetaData.GlData.SwapEffect,
            VSyncInterval = MetaData.WinData.VSyncInterval
        };

        return windowContextConfig;
    }

    public virtual void Prepare()
    {
        Establish();
    }

    protected virtual void Establish()
    {
        NativeWindow.Establish();
    }

    protected void ToggleFullscreen()
    {
        NativeWindow.ToggleFullscreen();
        MetaData.WinData.Fullscreen = NativeWindow.IsFullscreen;
    }

    protected void SetFullscreen(bool isFullscreen)
    {
        NativeWindow.SetFullscreen(isFullscreen);
        MetaData.WinData.Fullscreen = NativeWindow.IsFullscreen;
    }

    protected void SetOpacity(float opacity)
    {
        UpdateOpacity(opacity);
        NativeWindow.Opacity = MetaData.WinData.Opacity;
    }

    protected void UpdateOpacity(float opacity)
    {
        MetaData.WinData.Opacity = opacity;
    }

    protected void SetPositionAndSize(Vector2i size, Vector2i position)
    {
        SetSize(size);
        SetPosition(position);
    }

    protected void SetSize(Vector2i size)
    {
        UpdateSize(size);
        NativeWindow.SetSize(MetaData.WinData.Size);
    }

    protected void UpdateSize(Vector2i size)
    {
        MetaData.WinData.Size = size;
    }

    protected void SetPosition(Vector2i position)
    {
        UpdatePosition(position);
        NativeWindow.SetPosition(MetaData.WinData.Position);
    }

    protected void UpdatePosition(Vector2i position)
    {
        MetaData.WinData.Position = position;
    }

    protected void SetVsync(uint interval)
    {
        UpdateVsync(interval);
        NativeWindow.VSync = MetaData.WinData.VSyncInterval;
    }

    protected void UpdateVsync(uint interval)
    {
        MetaData.WinData.VSyncInterval = interval;
    }

    protected void UpdateViewport()
    {
        NativeWindow.DXContext.SetViewport(MetaData.GlData.Viewport);
    }

    protected void UpdateViewportDepth()
    {
        NativeWindow.DXContext.SetClipPlanes(MetaData.GlData.ClipPlanes);
    }

    protected void MoveOnDisplay(DisplayInfo display)
    {
        NativeWindow.Display = display;
    }

    protected void Show() => NativeWindow.Show();
    protected void Hide() => NativeWindow.Hide();
    protected void Maximize() => NativeWindow.Maximize();
    protected void Restore() => NativeWindow.Restore();
    protected void Commit() => NativeWindow.Update();

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
                UpdateSize(winResize.Size);
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

    protected void BeginFrame() => NativeWindow.BeginFrame();

    public virtual void Render()
    {
        Clear(MetaData.GlData.ClearColor);
    }

    protected void Clear(Color4 clearColor) => NativeWindow.Clear(clearColor);

    public virtual void PostRender()
    {
        EndFrame();
    }

    protected void EndFrame() => NativeWindow.EndFrame();

    public void Present() => NativeWindow.Present();

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