using OpenTK.Mathematics;
using MirageAPI.Window;

namespace Engine.Graphic.Window;

using Events;
using Events.QuantumEvents;
using Objects;
using Data.Meta;
using OpenGl;
using Logging;
using Data.RegistryManagers;

public class Window
    : MetaObject<WindowData>,
        IEventful, IUpdatable, IRenderable
{
    private readonly NativeWindow _nativeWindow;

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
        _nativeWindow.InitGLContext();
        
        QEventSystem.RegisterWindow(_nativeWindow);

        Registries.WindowRegistry.Register(this);
        // QuantumEventHandler.EventHandling += HandleEvent;
    }

    public void UpdateOpacity(float? opacity = null)
    {
        if (opacity.HasValue)
            MetaData.WinData.Opacity = opacity.Value;
    }

    public void UpdateSize(Vector2i? size = null)
    {
        if (size.HasValue)
            MetaData.WinData.Size = size.Value;
    }

    public void UpdatePosition(Vector2i? position = null)
    {
        if (position.HasValue)
            MetaData.WinData.Position = position.Value;
    }

    public virtual void HandleEvent()
    {
    }

    public virtual void HandleEvent(QuantumEvent e)
    {
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
    }

    public virtual void Render()
    {
    }

    public virtual void PostRender()
    {
    }

    public void Dispose()
    {
        Registries.WindowRegistry.Pop(Id);
        
        QEventSystem.UnregisterWindow(_nativeWindow);
        _nativeWindow.Dispose();

        Logger.Info($"Window '{Id}' disposed");
        GC.SuppressFinalize(this);
    }

    ~Window() => Dispose();
}