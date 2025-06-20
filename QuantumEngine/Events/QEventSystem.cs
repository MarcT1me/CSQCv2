using System.Collections.Concurrent;
using Engine.Logging;
using MirageAPI.Window;

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
        window.OnGlfwEvent += HandleGlfwEvent;
    }

    public static void UnregisterWindow(NativeWindow window)
    {
        window.OnGlfwEvent -= HandleGlfwEvent;
    }

    private static void HandleGlfwEvent(GlfwEvent glfwEvent)
    {
        Logger.Debug($"Event: {glfwEvent}");
        lock (Lock)
        {
            EventQueue.Enqueue(
                ConvertEvent(glfwEvent)
            );
        }
    }

    public static void PollEvents()
    {
        lock (Lock)
        {
            var handler = new EventBatchHandler();
            
            // Опрашиваем все окна
            NativeWindow.PollEvents();

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

    private static QuantumEvent ConvertEvent(GlfwEvent e)
    {
        return null!;
    }
}