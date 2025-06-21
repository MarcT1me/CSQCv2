using System.Collections.Concurrent;
// engine sub-systems
using MirageAPI.Window;
using MirageAPI.Events;

namespace Engine.Events;

using Data.RegistryManagers;
using QuantumEvents;
using Configuration;
using Threading;

public delegate void EventHandlingEvent(QuantumEvent e);

public class QEventSystem
{
    public static event EventHandlingEvent? EventHandling;
    private static readonly Lock Lock = new();

    private static readonly ConcurrentQueue<QuantumEvent> EventQueue = new();

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

    public static void RegisterWindow(NativeWindow window)
    {
        window.OnKey += HandleKeyEvent;
        window.OnMouse += HandleMouseEvent;
        window.OnWindow += HandleWindowEvent;
        window.OnChar += HandleCharEvent;
        window.OnDrop += HandleDropEvent;
    }

    public static void UnregisterWindow(NativeWindow window)
    {
        window.OnKey -= HandleKeyEvent;
        window.OnMouse -= HandleMouseEvent;
        window.OnWindow += HandleWindowEvent;
        window.OnChar -= HandleCharEvent;
        window.OnDrop -= HandleDropEvent;
    }

    private static void HandleKeyEvent(NativeKeyEvent glfwEvent)
    {
        EnqueueEvent(ConvertEvent(glfwEvent));
    }

    private static void HandleMouseEvent(NativeMouseEvent glfwEvent)
    {
        EnqueueEvent(ConvertEvent(glfwEvent));
    }

    private static void HandleWindowEvent(NativeWindowEvent glfwEvent)
    {
        EnqueueEvent(ConvertEvent(glfwEvent));
    }

    private static void HandleCharEvent(NativeCharEvent glfwEvent)
    {
        EnqueueEvent(ConvertEvent(glfwEvent));
    }

    private static void HandleDropEvent(NativeDropEvent glfwEvent)
    {
        EnqueueEvent(ConvertEvent(glfwEvent));
    }

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
            var handler = new EventBatchHandler();

            // Опрашиваем все окна
            NativeEventManager.PollEvents();

            // Обрабатываем накопленные события
            while (EventQueue.TryDequeue(out var qEvent))
            {
                UpdateInputState(qEvent);
                handler.Handle(qEvent);
            }

            handler.Dispose();
        }
    }

    private static void UpdateInputState(QuantumEvent qEvent)
    {
    }

    private static QuantumEvent ConvertEvent(NativeKeyEvent e) => new KeyEvent(e);

    private static QuantumEvent ConvertEvent(NativeMouseEvent e) => e.Type switch
    {
        NativeMouseEventType.Button => new QuantumEvents.Mouse.ButtonEvent(e),
        NativeMouseEventType.Move => new QuantumEvents.Mouse.MoveEvent(e),
        NativeMouseEventType.Scroll => new QuantumEvents.Mouse.ScrollEvent(e),
        _ => new QuantumEvent(EventType.Unknown)
    };

    private static QuantumEvent ConvertEvent(NativeWindowEvent e)
    {
        switch (e.Type)
        {
            case NativeWindowEventType.Close:
            {
                if (Registries.WindowRegistry.Size == 1)
                    EnqueueEvent(new WindowedQuantumEvent(
                        EventType.Quit,
                        e.windowID
                    ));
                return new WindowedQuantumEvent(
                    EventType.WindowClose,
                    e.windowID
                );
            }
            case NativeWindowEventType.Focus:
                return new WindowedQuantumEvent(
                    e.X == 1 ? EventType.WindowFocusGained : EventType.WindowFocusLost,
                    e.windowID
                );
            case NativeWindowEventType.Maximize:
                return new WindowedQuantumEvent(
                    e.X == 1 ? EventType.WindowMaximize : EventType.WindowMinimize,
                    e.windowID
                );
            case NativeWindowEventType.Resize:
                return new QuantumEvents.Window.ResizeEvent(e);
            case NativeWindowEventType.Move:
                return new QuantumEvents.Window.MoveEvent(e);
            case NativeWindowEventType.Iconify:
                return new QuantumEvents.Window.IconifyEvent(e);
            default:
                return new QuantumEvent(EventType.Unknown);
        }
    }

    private static QuantumEvent ConvertEvent(NativeCharEvent e) => new CharEvent(e);

    private static QuantumEvent ConvertEvent(NativeDropEvent e) => new DropEvent(e);
}