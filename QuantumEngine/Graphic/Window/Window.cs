using Engine.Events.QuantumEvents.Window;
using MirageAPI;
using MirageAPI.DirectX;
using OpenTK.Mathematics;
// engine sub-systems
using MirageAPI.Window;

namespace Engine.Graphic.Window;

using Events;
using Events.QuantumEvents;
using Objects;
using Data.Meta;
using Logging;
using Data.RegistryManagers;

public class Window
    : MetaObject<WindowData>, IPreparableInstance<NativeWindow>, IPreparable,
        IEventful, IUpdatable, IRenderable
{
    protected readonly NativeWindow NativeWindow;
    public const int UseDefault = unchecked((int)0x80000000);

    public Window? ActiveWindow { get; protected set; }

    public Window? ParentWindow { get; init; }
    public IntPtr Handle => NativeWindow.Handle;
    public ObjectStatusFlags ObjectStatus => MetaData.Status;

    public Window(
        WinData winData,
        GlData? glData = null,
        string? name = null,
        Window? parent = null
    ) : base(new WindowData(winData, glData ?? new GlData(), name))
    {
        Logger.Info(
            $"Creating window '{MetaData.Identifier}'\n" +
            $"Position: {winData.Position}\n" +
            $"Size: {winData.Size}\n" +
            $"Opacity: {winData.Opacity}"
        );

        // ReSharper disable once VirtualMemberCallInConstructor
        NativeWindow = PrepareInstance();
        ParentWindow = parent;

        QEventSystem.RegisterWindow(NativeWindow);
        QEventSystem.EventHandling += HandleEvent;

        Input.Mouse.Mouse.RegisterWindow(this);
        Input.Keyboard.Keyboard.RegisterWindow(this);

        Registries.WindowRegistry.Register(this);
    }

    public virtual NativeWindow PrepareInstance()
    {
        var win = new NativeWindow(
            new SimpleRect
            {
                X = MetaData.WinData.Position.X,
                Y = MetaData.WinData.Position.Y,
                Width = MetaData.WinData.Size.X,
                Height = MetaData.WinData.Size.Y
            },
            MetaData.Identifier.GetNameAnyway(),
            MetaData.WinData.Opacity,
            false,
            ParentWindow?.NativeWindow,
            WindowType.Overlapped,
            CreateDefaultWindowContextConfig()
        );
        return win;
    }

    protected DX12ContextConfig CreateDefaultWindowContextConfig()
    {
        var windowContextConfig = new DX12ContextConfig
        {
            Viewport = new SimpleRect
            {
                X = MetaData.GlData.Viewport.X,
                Y = MetaData.GlData.Viewport.Y,
                Width = MetaData.GlData.Viewport.Z,
                Height = MetaData.GlData.Viewport.W
            },
            ResolutionX = (uint)MetaData.WinData.Resolution.X,
            ResolutionY = (uint)MetaData.WinData.Resolution.Y,
            Near = MetaData.GlData.ViewportDepth.X,
            Far = MetaData.GlData.ViewportDepth.Y,

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

    protected void TogleFullscreen()
    {
        MetaData.WinData.Fullscreen = !MetaData.WinData.Fullscreen;
        NativeWindow.ToggleFullscreen();
    }

    protected void SetOpacity(float? opacity = null)
    {
        if (opacity.HasValue)
            UpdateOpacity(opacity.Value);
        NativeWindow.SetOpacity(MetaData.WinData.Opacity);
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

    protected void SetSize(Vector2i? size = null)
    {
        if (size.HasValue)
            UpdateSize(size.Value);
        NativeWindow.SetSize(MetaData.WinData.Size.X, MetaData.WinData.Size.Y);
    }

    protected void UpdateSize(Vector2i size)
    {
        MetaData.WinData.Size = size;
    }

    protected void SetPosition(Vector2i? position = null)
    {
        if (position.HasValue)
            UpdatePosition(position.Value);
        NativeWindow.SetPosition(MetaData.WinData.Position.X, MetaData.WinData.Position.Y);
    }

    protected void UpdatePosition(Vector2i position)
    {
        MetaData.WinData.Position = position;
    }

    protected void SetVsync(uint? interval = 1)
    {
        if (interval.HasValue)
            UpdateVsync(interval.Value);
        NativeWindow.SetVSync(MetaData.WinData.VSyncInterval);
    }

    protected void UpdateVsync(uint interval)
    {
        MetaData.WinData.VSyncInterval = interval;
    }

    protected void UpdateViewport()
    {
        NativeWindow.DXContext.SetViewport(
            MetaData.GlData.Viewport.X,
            MetaData.GlData.Viewport.Y,
            MetaData.GlData.Viewport.Z,
            MetaData.GlData.Viewport.W
        );
    }

    protected void UpdateViewportDepth()
    {
        NativeWindow.DXContext.SetViewportDepth(
            MetaData.GlData.ViewportDepth.X,
            MetaData.GlData.ViewportDepth.Y
        );
    }

    protected void BeginFrame() => NativeWindow.BeginFrame();

    protected void Clear(Vector4 clearColor) =>
        NativeWindow.Clear(clearColor.X, clearColor.Y, clearColor.Z, clearColor.W);

    protected void EndFrame() => NativeWindow.EndFrame();
    protected void Present() => NativeWindow.Present();

    protected void Show() => NativeWindow.Show();
    protected void Hide() => NativeWindow.Hide();
    protected void Maximize() => NativeWindow.Maximize();
    protected void Restore() => NativeWindow.Restore();
    protected void Commit() => NativeWindow.Update();

    public virtual void HandleEvent(QuantumEvent e)
    {
        if (e is { Type: EventType.WindowClose })
            Dispose();
        else if (e.Type == EventType.WindowFocusGained)
            ActiveWindow = this;
        else if (e.Type == EventType.WindowFocusLost && ActiveWindow == this)
            ActiveWindow = null;
        else if (e is WinResizeEvent winResize)
            HandleResize(winResize.Size);
    }

    protected virtual void HandleResize(Vector2i size)
    {
        UpdateSize(size);
        UpdateViewport();
        NativeWindow.HandleResize(MetaData.WinData.Resolution.X, MetaData.WinData.Resolution.Y);
    }

    public virtual void PreUpdate()
    {
    }

    public virtual void Update()
    {
    }

    public virtual void PostUpdate()
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
        Present();
    }

    public virtual void Dispose()
    {
        Registries.WindowRegistry.Pop(Handle);

        QEventSystem.UnregisterWindow(NativeWindow);
        QEventSystem.EventHandling -= HandleEvent;

        Input.Mouse.Mouse.UnregisterWindow(this);
        Input.Keyboard.Keyboard.UnregisterWindow(this);

        NativeWindow.Dispose();

        Logger.Info($"Window '{Id}' disposed");
        GC.SuppressFinalize(this);
    }

    ~Window() => Dispose();
}