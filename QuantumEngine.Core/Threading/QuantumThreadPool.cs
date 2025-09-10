namespace QuantumCore.Threading;

using Data;

public sealed class QuantumThreadPool : ThreadRoster
{
    private readonly CountdownEvent _countdownEvent = new(0);
    private bool _isLaunched;
    private bool _isDisposed;

    public void QueueWorkItem(Action workItem)
    {
        if (_isLaunched || _isDisposed)
            throw new InvalidOperationException("Thread pool is not active");

        lock (this)
        {
            var id = Identifier.FromUncertain(workItem);

            Pending[id] = new QuantumThread(
                name: $"PoolThread-{id}",
                isBackground: true,
                action: () =>
                {
                    try
                    {
                        workItem();
                    }
                    finally
                    {
                        lock (this)
                        {
                            _countdownEvent.Signal();
                        }
                    }
                }
            );
            _countdownEvent.AddCount();
        }
    }

    public void Launch()
    {
        if (_isLaunched || _isDisposed)
            throw new InvalidOperationException("Thread pool already launched or disposed");

        lock (this)
        {
            _isLaunched = true;

            foreach (var id in Pending.Keys.ToList())
            {
                if (Pending[id] is not { } thread) continue;

                Worked[id] = thread;
                Pending.Pop(id);
                thread.Start();
            }

            _countdownEvent.Wait();
            Cleanup();
        }
    }

    public override void Dispose()
    {
        if (_isDisposed) return;
        _isDisposed = true;
        foreach (QuantumThread thread in Worked.Values)
        {
            thread.Join(0f);
        }

        Cleanup();
        base.Dispose();
    }

    ~QuantumThreadPool() => Dispose();
}