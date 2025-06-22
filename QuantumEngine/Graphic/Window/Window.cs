using OpenTK.Mathematics;
// engine sub-systems
using MirageAPI;
using MirageAPI.Window;

namespace Engine.Graphic.Window;

using Events;
using Events.QuantumEvents;
using Events.QuantumEvents.Window;
using Objects;
using Data.Meta;
using Logging;
using Data.RegistryManagers;

public class Window
    : MetaObject<WindowData>,
        IEventful, IUpdatable, IRenderable
{
    private readonly NativeWindow _nativeWindow;
    // private readonly VulkanRenderer _vulkanRenderer;

    public Window? ActiveWindow { get; protected set; }

    public IntPtr Handle => _nativeWindow.Handle;
    public ObjectStatusFlags ObjectStatus => MetaData.Status;

    public Window(
        WinData winData,
        GlData? glData = null,
        string? name = null
    ) : base(new WindowData(winData, glData ?? new GlData(), name))
    {
        Logger.Info(
            $"Creating window '{MetaData.Identifier}'\n" +
            $"Position: {winData.Position}\n" +
            $"Size: {winData.Size}\n" +
            $"Opacity: {winData.Opacity}"
        );

        _nativeWindow = new NativeWindow(
            winData.Size.X,
            winData.Size.Y,
            MetaData.Identifier.GetNameAnyway(),
            null
        );
        SetOpacity(winData.Opacity);
        SetPosition(winData.Position);

        if (MirageSystem.initVulkan)
        {
            unsafe
            {
                _nativeWindow.CreateVulkanSurface(MirageSystem.CurrentContext.Instance);
            }
        }
        else if (MirageSystem.initOpenGl)
        {
            SetViewport(winData.Size);
        }

        // _vulkanRenderer = new VulkanRenderer(
        // MirageSystem.CurrentContext,
        // QEngineCore.VulkanPipeline,
        // _nativeWindow
        // );

        QEventSystem.RegisterWindow(_nativeWindow);
        QEventSystem.EventHandling += HandleEvent;

        Input.Mouse.Mouse.RegisterWindow(this);
        Input.Keyboard.Keyboard.RegisterWindow(this);

        Registries.WindowRegistry.Register(this);
    }

    public void SetOpacity(float? opacity = null)
    {
        if (opacity.HasValue)
            UpdateOpacity(opacity.Value);
        _nativeWindow.SetOpacity(MetaData.WinData.Opacity);
    }

    public void UpdateOpacity(float opacity)
    {
        MetaData.WinData.Opacity = opacity;
    }

    public void SetSizeLimits(Vector2i? minSize = null, Vector2i? maxSize = null)
    {
        if (minSize.HasValue)
            MetaData.WinData.MinSize = minSize.Value;
        if (maxSize.HasValue)
            MetaData.WinData.MinSize = maxSize.Value;
        _nativeWindow.SetSizeLimit(
            MetaData.WinData.MinSize.X, MetaData.WinData.MinSize.Y,
            MetaData.WinData.MaxSize.X, MetaData.WinData.MaxSize.Y
        );
        SetSize();
    }

    public void SetSize(Vector2i? size = null)
    {
        if (size.HasValue)
            UpdateSize(size.Value);
        _nativeWindow.SetSize(MetaData.WinData.Size.X, MetaData.WinData.Size.Y);
    }

    public void UpdateSize(Vector2i size)
    {
        MetaData.WinData.Size = size;
    }

    public void UpdateSizeWithChain(Vector2i size)
    {
        UpdateSize(size);
        if (MirageSystem.initVulkan)
        {
            // _vulkanRenderer.RecreateSwapChain();
        }
        else if (MirageSystem.initOpenGl)
        {
            SetViewport(size);
        }
    }

    public void SetViewport(Vector2i size)
    {
        MakeCurrent();
        NativeWindow.SetViewport(0, 0, size.X, size.Y);
    }

    public void SetPosition(Vector2i? position = null)
    {
        if (position.HasValue)
            UpdatePosition(position.Value);
        _nativeWindow.SetPos(MetaData.WinData.Position.X, MetaData.WinData.Position.Y);
    }

    public void UpdatePosition(Vector2i position)
    {
        MetaData.WinData.Position = position;
    }

    public void Clear(Vector4 clearColor)
    {
        MakeCurrent();
        NativeWindow.Clear(clearColor.X, clearColor.Y, clearColor.Z, clearColor.W);
    }

    public void Focus() => _nativeWindow.Focus();
    public void Show() => _nativeWindow.Show();
    public void Hide() => _nativeWindow.Hide();
    public void Maximize() => _nativeWindow.Maximize();
    public void Restore() => _nativeWindow.Restore();

    public void SwapBuffers() => _nativeWindow.SwapBuffers();
    public void MakeCurrent() => _nativeWindow.MakeCurrent();
    public bool ShouldClose() => _nativeWindow.ShouldClose();

    public virtual void HandleEvent(QuantumEvent e)
    {
        if (e is { Type: EventType.WindowClose } || ShouldClose())
            Dispose();
        else if (e is WinResizeEvent resize)
            UpdateSizeWithChain(resize.Size);
        else if (e.Type == EventType.WindowFocusGained)
            ActiveWindow = this;
        else if (e.Type == EventType.WindowFocusLost && ActiveWindow == this)
            ActiveWindow = null;
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
        Clear(MetaData.GlData.ClearColor);

        // _vulkanRenderer.DrawFrame(
        //     clearColor.X,
        //     clearColor.Y,
        //     clearColor.Z,
        //     clearColor.W
        // );
    }

    public virtual void Render()
    {
    }

    public virtual void PostRender()
    {
        SwapBuffers();
    }

    public void Dispose()
    {
        Registries.WindowRegistry.Pop(Handle);

        QEventSystem.UnregisterWindow(_nativeWindow);
        QEventSystem.EventHandling -= HandleEvent;

        Input.Mouse.Mouse.UnregisterWindow(this);
        Input.Keyboard.Keyboard.UnregisterWindow(this);

        // _vulkanRenderer.Dispose();

        if (MirageSystem.initVulkan)
        {
            unsafe
            {
                _nativeWindow.CleanupVulkanSurface(MirageSystem.CurrentContext.Instance);
            }
        }

        _nativeWindow.Dispose();

        Logger.Info($"Window '{Id}' disposed");
        GC.SuppressFinalize(this);
    }

    ~Window() => Dispose();
}