using Engine.Configuration;
using Engine.Events.QuantumEvents.Window;
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
            new WindowRect
            {
                x = MetaData.WinData.Position.X,
                y = MetaData.WinData.Position.Y,
                width = MetaData.WinData.Size.X,
                height = MetaData.WinData.Size.Y
            },
            MetaData.Identifier.GetNameAnyway(),
            MetaData.WinData.Opacity,
            ParentWindow?.NativeWindow,
            WindowType.Overlapped,
            CreateDefaultWindowContextConfig()
        );
        return win;
    }

    protected DX12WindowContextConfig CreateDefaultWindowContextConfig()
    {
        var windowContextConfig = DX12WindowContextConfig.Default;
        windowContextConfig.EnableDebugLayer = GlData.EnableDebugLayer && BaseConfig.DebugMode;
        windowContextConfig.AllowTearing = GlData.AllowTearing && BaseConfig.DebugMode;
        
        windowContextConfig.BufferCount = GlData.MaxFramesInFlight;
        windowContextConfig.SwapEffect = GlData.SwapEffect;
        windowContextConfig.SampleCount = GlData.NumberOfSamples;
        windowContextConfig.SwapQuality = GlData.SwapQuality;

        windowContextConfig.viewportX = MetaData.GlData.Viewport.X;
        windowContextConfig.viewportY = MetaData.GlData.Viewport.Y;
        windowContextConfig.viewportWidth = MetaData.GlData.Viewport.Z;
        windowContextConfig.viewportHeight = MetaData.GlData.Viewport.W;

        windowContextConfig.viewportDepthX = MetaData.GlData.ViewportDepth.X;
        windowContextConfig.viewportDepthY = MetaData.GlData.ViewportDepth.Y;

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

    protected void SetPositionAndSize(Vector2i size, Vector2i position)
    {
        SetSize(size);
        SetPosition(position);
    }

    protected void SetVsync(bool enabled = true)
    {
        NativeWindow.SetVSync(enabled);
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
        MetaData.GlData.Viewport.Z = size.X;
        MetaData.GlData.Viewport.W = size.Y;
        UpdateSize(size);
        UpdateViewport();
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