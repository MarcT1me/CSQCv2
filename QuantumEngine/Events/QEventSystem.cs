using System.Collections.Concurrent;

// engine sub-systems

namespace Engine.Events;

using Data.RegistryManagers;
using QuantumEvents;
using Threading;
using QuantumEvents.Joystick;
using QuantumEvents.Mouse;
using Input.Joystick;
using Input.Keyboard;
using Input.Mouse;

public delegate void EventHandlingEvent(QuantumEvent e);

public class QEventSystem
{
    public static event EventHandlingEvent? EventHandling;
    private static readonly Lock Lock = new();

    private static readonly ConcurrentQueue<QuantumEvent> EventQueue = new();

    private class EventBatchHandler : IDisposable
    {
        private readonly QuantumThreadPool? _threadPool;

        private static readonly bool IsMultiThreadEventHandling =
            EngineCore.Core.Configuration.Get<bool>("engine.threading.multithreadEvents");

        public EventBatchHandler()
        {
            if (IsMultiThreadEventHandling)
                _threadPool = new QuantumThreadPool();
        }

        public void Handle(QuantumEvent qEvent)
        {
            if (IsMultiThreadEventHandling)
            {
                _threadPool?.QueueWorkItem(() => EventHandling?.Invoke(qEvent));
            }
            else
            {
                EventHandling?.Invoke(qEvent);
            }
        }

        public void Dispose()
        {
            _threadPool?.Launch();
            _threadPool?.Dispose();
        }
    }

    static QEventSystem()
    {
        MirageAPI.Events.EventManager.OnKey += HandleKeyEvent;
        MirageAPI.Mouse.OnMouse += HandleMouseEvent;
    }

    public static void RegisterWindow(MirageAPI.Window window)
    {
        window.OnWindow += HandleWindowEvent;
    }

    public static void UnregisterWindow(MirageAPI.Window window)
    {
        window.OnWindow += HandleWindowEvent;
    }

    private static void HandleKeyEvent(MirageAPI.Events.KeyEvent nativeEvent) =>
        EnqueueEvent(ConvertEvent(nativeEvent));

    private static void HandleMouseEvent(MirageAPI.Events.MouseEvent nativeEvent) =>
        EnqueueEvent(ConvertEvent(nativeEvent));

    private static void HandleWindowEvent(MirageAPI.Events.WindowEvent nativeEvent) =>
        EnqueueEvent(ConvertEvent(nativeEvent));

    public static void EnqueueEvent(QuantumEvent qEvent)
    {
        lock (Lock)
        {
            EventQueue.Enqueue(qEvent);
        }
    }

    public static void PollEvents()
    {
        lock (Lock)
        {
            using var handler = new EventBatchHandler();

            // Опрашиваем все окна
            MirageAPI.Events.EventManager.ProcessEvents();

            // Обрабатываем накопленные события
            while (EventQueue.TryDequeue(out var qEvent))
            {
                handler.Handle(qEvent);

                if (qEvent is WindowedQuantumEvent { Type: EventType.WindowClose } wEvent
                    && Registries.WindowRegistry.Size == 0)
                {
                    EnqueueEvent(new WindowedQuantumEvent(EventType.Quit, wEvent.WindowId));
                }

                UpdateInputState(qEvent);
            }
        }
    }

    private static void UpdateInputState(QuantumEvent qEvent)
    {
        switch (qEvent)
        {
            case MouseEvent mouseEvent:
                Mouse.Update(mouseEvent);
                break;
            case KeyEvent keyEvent:
                Keyboard.Update(keyEvent);
                break;
            case JoyEvent joyEvent:
                Joy.Update(joyEvent);
                break;
        }
    }

    private static QuantumEvent ConvertEvent(MirageAPI.Events.KeyEvent e) => new KeyEvent(e);

    private static QuantumEvent ConvertEvent(MirageAPI.Events.MouseEvent e) => e.Type switch
    {
        MirageAPI.Events.EventType.MouseButton => new MouseButtonEvent(e),
        MirageAPI.Events.EventType.MouseMove => new MouseMoveEvent(e),
        MirageAPI.Events.EventType.MouseScroll => new MouseScrollEvent(e),
        MirageAPI.Events.EventType.MouseEnter => new MouseEvent(EventType.MouseEnter, e.windowID),
        MirageAPI.Events.EventType.MouseLeave => new MouseEvent(EventType.MouseLeave, e.windowID),
        _ => new QuantumEvent(EventType.Unknown)
    };

    private static QuantumEvent ConvertEvent(MirageAPI.Events.WindowEvent e) => e.Type switch
    {
        MirageAPI.Events.EventType.WindowCreate => new WindowedQuantumEvent(
            EventType.WindowClose,
            e.windowID
        ),
        MirageAPI.Events.EventType.WindowClose => new WindowedQuantumEvent(
            EventType.WindowClose,
            e.windowID
        ),
        MirageAPI.Events.EventType.WindowDestroy => new WindowedQuantumEvent(
            EventType.WindowClose,
            e.windowID
        ),
        MirageAPI.Events.EventType.WindowFocus => new WindowedQuantumEvent(
            e.X == 1 ? EventType.WindowFocusGained : EventType.WindowFocusLost,
            e.windowID
        ),
        MirageAPI.Events.EventType.WindowResize => new QuantumEvents.Window.WinResizeEvent(e),
        MirageAPI.Events.EventType.WindowMove => new QuantumEvents.Window.WinMoveEvent(e),
        _ => new QuantumEvent(EventType.Unknown)
    };
}