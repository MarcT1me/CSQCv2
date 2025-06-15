// ReSharper disable RedundantUnsafeContext

using SDL2;

namespace Engine.Events;

using QuantumEvents;
using Configuration;
using Threading;

public class QuantumEventHandler
{
    public static event HandleEvent? OnEvent;
    private static readonly Lock Lock = new();

    private class EventBatchHandler : IDisposable
    {
        private readonly QuantumThreadPool? _threadPool;

        public EventBatchHandler()
        {
            if (ThreadingConfig.IsMultiThreadEventHanlding)
                _threadPool = new QuantumThreadPool();
        }

        public void Handle(QuantumEvent qEvent)
        {
            if (ThreadingConfig.IsMultiThreadEventHanlding)
            {
                _threadPool?.QueueWorkItem(() => OnEvent?.Invoke(qEvent));
            }
            else
            {
                OnEvent?.Invoke(qEvent);
            }
        }

        public void Dispose()
        {
            _threadPool?.Launch();
            _threadPool?.Dispose();
        }
    }

    public static void HandleEvents()
    {
        lock (Lock)
        {
            var handler = new EventBatchHandler();
            while (SDL.SDL_PollEvent(out var sdlEvent) != 0)
            {
                var qEvent = ConvertEvent(sdlEvent);
                UpdateInputState(qEvent);
                handler.Handle(qEvent);
            }
            handler.Dispose();
        }
    }

    private static void UpdateInputState(QuantumEvent qEvent)
    {
        switch (qEvent)
        {
            // check Input events
            case QuantumEvents.Mouse.MouseEvent me:
                Input.Mouse.Mouse.Update(me);
                break;
            case KeyboardEvent ke:
                Input.Keyboard.Keyboard.Update(ke);
                break;
            case QuantumEvents.Joy.JoyEvent je:
                Input.Controller.Joy.Update(je);
                break;
            case QuantumEvents.Controller.ControllerEvent ce:
                Input.Controller.Controller.Update(ce);
                break;
        }
    }

    private static unsafe QuantumEvent ConvertEvent(SDL.SDL_Event e)
    {
        var eventType = (EventType)e.type;

        return eventType switch
        {
            EventType.ControllerAxisMotion => new QuantumEvents.Controller.AxisEvent(e),
            EventType.ControllerButtonDown or EventType.ControllerButtonUp =>
                new QuantumEvents.Controller.ButtonEvent(e),
            EventType.ControllerDeviceAdded or EventType.ControllerDeviceRremapped or EventType.ControllerDeviceRremoved
                => new QuantumEvents.Controller.DeviceEvent(e),

            EventType.JoyAxisMotion => new QuantumEvents.Joy.AxisEvent(e),
            EventType.JoyBallMotion => new QuantumEvents.Joy.BallEvent(e),
            EventType.JoyButtonDown or EventType.JoyButtonUp => new QuantumEvents.Joy.ButtonEvent(e),
            EventType.JoyHatMotion => new QuantumEvents.Joy.HatEvent(e),
            EventType.JoyDeviceAdded or EventType.JoyDeviceRemoved => new QuantumEvents.Joy.DeviceEvent(e),

            EventType.KeyDown or EventType.KeyUp => new KeyboardEvent(e),

            EventType.MouseButtonDown or EventType.MouseButtonUp => new QuantumEvents.Mouse.ButtonEvent(e),
            EventType.MouseMotion => new QuantumEvents.Mouse.MotionEvent(e),
            EventType.MouseWheel => new QuantumEvents.Mouse.WheelEvent(e),

            EventType.AudioDeviceAdded or EventType.AudioDeviceRemoved => new AudioDeviceEvent(e),
            EventType.DropFile or EventType.DropText => new DropEvent(e),
            EventType.Textinput => new TextInputEvent(e),
            EventType.TextEditing => new TextEditingEvent(e),
            EventType.WindowEvent => new QuantumEvents.Window.WindowEvent(e),
            EventType.UserEvent => new UserEvent(e),

            EventType.Quit => new QuantumEvent(e, e.quit.timestamp),
            _ => new QuantumEvent(e, 0)
        };
    }
}