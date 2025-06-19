using OpenTK.Mathematics;
using SDL2;
using MirageAPI.Window;

namespace Engine.Graphic.Window;

using Events;
using Events.QuantumEvents.Window;
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
    public const int UndefinedMask = 536805376;
    public const int CenteredMask = 805240832;
    public const int Undefined = 536805376;
    public const int Centered = 805240832;

    private readonly IntPtr _window;

    public ObjectStatusFlags ObjectStatus => MetaData.Status;
    public uint WinId => SDL.SDL_GetWindowID(_window);

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

        var nativeWindow = new NativeWindow(
            winData.Size.X,
            winData.Size.Y,
            MetaData.Identifier.GetNameAnyway(),
            null
        );

        IntPtr nativeHandle = nativeWindow.Handle;
        _window = SDL.SDL_CreateWindowFrom(nativeHandle);
        nativeWindow.InitGLContext();

        if (_window == IntPtr.Zero)
        {
            throw new Exception("Failed to create window: " + SDL.SDL_GetError());
        }

        Registries.WindowRegistry.Register(this);
        QuantumEventHandler.EventHandling += HandleEvent;
    }

    public void Show() => SDL.SDL_ShowWindow(_window);
    public void Hide() => SDL.SDL_HideWindow(_window);

    public void Raise() => SDL.SDL_RaiseWindow(_window);
    public void Minimize() => SDL.SDL_MinimizeWindow(_window);
    public void Maximize() => SDL.SDL_MaximizeWindow(_window);

    public void UpdateOpacity(float? opacity = null)
    {
        if (opacity.HasValue)
            MetaData.WinData.Opacity = opacity.Value;
        SDL.SDL_SetWindowOpacity(_window, MetaData.WinData.Opacity);
    }

    public void UpdateSize(Vector2i? size = null)
    {
        if (size.HasValue)
            MetaData.WinData.Size = size.Value;
        SDL.SDL_SetWindowSize(_window, MetaData.WinData.Size.X, MetaData.WinData.Size.Y);
    }

    public void UpdatePosition(Vector2i? position = null)
    {
        if (position.HasValue)
            MetaData.WinData.Position = position.Value;
        SDL.SDL_SetWindowPosition(_window, MetaData.WinData.Position.X, MetaData.WinData.Position.Y);
    }

    public void SwapBuffers() => SDL.SDL_GL_SwapWindow(_window);

    public void Close() => SDL.SDL_DestroyWindow(_window);

    public void HandleEvent(QuantumEvent e)
    {
        if (e is not WindowEvent windowEvent || windowEvent.WindowId != WinId) return;

        switch (windowEvent.windowEvent)
        {
            case WindowEventId.FocusLost:
                SDL.SDL_StopTextInput();
                break;
            case WindowEventId.FocusGained:
                SDL.SDL_StartTextInput();
                break;
            case WindowEventId.SizeChanged:
                UpdateSize(new(windowEvent.Data1, windowEvent.Data2));
                break;
            case WindowEventId.Moved:
                MetaData.WinData.Position = new(windowEvent.Data1, windowEvent.Data2);
                break;
            case WindowEventId.Close:
                Dispose();
                break;
        }
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

    public void PreRender()
    {
        // GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
    }

    public void Render()
    {
    }

    public void PostRender()
    {
        SwapBuffers();
    }

    public void Dispose()
    {
        Close();
        Registries.WindowRegistry.Pop(Id);

        QuantumEventHandler.EventHandling -= HandleEvent;

        Logger.Info($"Window '{Id}' disposed");

        GC.SuppressFinalize(this);
    }

    ~Window() => Dispose();
}