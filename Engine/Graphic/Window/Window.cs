using Engine.Events.QuantumEvents.Window;
using OpenTK.Graphics.OpenGL;
using SDL2;

namespace Engine.Graphic.Window;

using Events.QuantumEvents;
using Objects;
using Data.Meta;
using OpenGl;
using Logging;

public class Window 
    : MetaObject<WindowData>,
        IEventful, IRenderable, IDisposable
{
    public const int UndefinedMask = 536805376;
    public const int CenteredMask = 805240832;
    public const int Undefined = 536805376;
    public const int Centered = 805240832;

    private readonly IntPtr _window;
    private readonly IntPtr _glContext;

    public static void InitialiseSdl()
    {
        SDL.SDL_Init(SDL.SDL_INIT_VIDEO);
    }

    public static void UnInitialiseSdl()
    {
        SDL.SDL_Quit();
    }

    public ObjectStatusFlags ObjectStatus => MetaData.Status;
    public uint WinId => SDL.SDL_GetWindowID(_window);

    public Window(
        string name,
        WinData winData,
        GlData? glData = null
    ) : base(new WindowData(winData, glData ?? new GlData(), name))
    {
        Logger.Info(
            $"Creating window '{name}'" +
            $"Position: {winData.Position}" +
            $"Size: {winData.Size}" +
            $"Opacity: {winData.Opacity}"
        );

        _window = SDL.SDL_CreateWindow(
            name,
            winData.Position.X, winData.Position.Y,
            winData.Size.X, winData.Size.Y,
            (SDL.SDL_WindowFlags)(winData.Flags | WinFlags.Opengl | WinFlags.Hidden)
        );
        if (_window == IntPtr.Zero)
        {
            throw new Exception("Failed to create window: " + SDL.SDL_GetError());
        }

        _glContext = SDL.SDL_GL_CreateContext(_window);
        if (_glContext == IntPtr.Zero)
        {
            throw new Exception("Failed to create OpenGL context: " + SDL.SDL_GetError());
        }
    }

    public void Show() => SDL.SDL_ShowWindow(_window);
    public void Hide() => SDL.SDL_HideWindow(_window);

    public void Raise() => SDL.SDL_RaiseWindow(_window);
    public void Minimize() => SDL.SDL_MinimizeWindow(_window);
    public void Maximize() => SDL.SDL_MaximizeWindow(_window);

    public void SetOpacity(float opacity) => SDL.SDL_SetWindowOpacity(_window, opacity);

    public void SetCurrent() => SDL.SDL_GL_MakeCurrent(_window, _glContext);
    public void SwapBuffers() => SDL.SDL_GL_SwapWindow(_window);

    public void DeleteContext() => SDL.SDL_GL_DeleteContext(_glContext);
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
            case WindowEventId.Close:
                Dispose();
                break;
        }
    }

    public void PreRender()
    {
        SetCurrent();
        GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
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
        DeleteContext();
        Close();
        GC.SuppressFinalize(this);
    }
    
    ~Window() => Dispose();
}