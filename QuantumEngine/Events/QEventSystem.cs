using System.Collections.Concurrent;
using Engine.Data.RegistryManagers;
using Engine.Logging;
// engine sub-systems
using MirageAPI.Window;
using MirageAPI.Events;

namespace Engine.Events;

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

    private static QuantumEvent ConvertEvent(NativeWindowEvent e)
    {
        if (e.Type == NativeWindowEventType.Close)
        {
            if (Registries.WindowRegistry.Size == 1)
                EnqueueEvent(new QuantumEvent(EventType.Quit));
            return new QuantumEvent(EventType.WindowClose);
        }

        return null!;
    }

    private static QuantumEvent ConvertEvent(NativeKeyEvent e)
    {
        return null!;
    }

    private static QuantumEvent ConvertEvent(NativeMouseEvent e)
    {
        return null!;
    }

    private static QuantumEvent ConvertEvent(NativeCharEvent e)
    {
        return null!;
    }

    private static QuantumEvent ConvertEvent(NativeDropEvent e)
    {
        return null!;
    }
}