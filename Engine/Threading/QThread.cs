namespace Engine.Threading;

using Data;
using Data.Meta;
using Failures;

public class QThread : MetaObject<QThreadMeta>, IDisposable, IFailureHandler
{
    #region Static Members

    private static readonly ThreadRoster Roster = new();
    private static readonly Lock GlobalLock = new();
    private static readonly object ImportantLock = new();

    private static Identifier? _importantThreadId;

    public static Identifier? ImportantThreadId
    {
        get => _importantThreadId;
        set
        {
            lock (ImportantLock)
            {
                _importantThreadId = value;
                Monitor.PulseAll(ImportantLock);
            }
        }
    }

    public static QThread? Current
    {
        get
        {
            Identifier? identifier = Identifier.GiveFromUncertain(Thread.CurrentThread.Name!);
            if (identifier == null) return null;
            return Roster.Worked[identifier];
        }
    }

    #endregion

    #region props and incapsulations

    private readonly Thread _thread;
    private object? _actionResult;
    private bool _isDisposed;

    public bool IsAlive => _thread.IsAlive;
    public object? Result => _actionResult;
    public ThreadState State => _thread.ThreadState;

    #endregion

    protected QThread(
        string? name = null,
        FailureLevel? failureLevel = null,
        bool isBackground = true,
        float? lifetimeSeconds = null
    ) : base(new QThreadMeta(name, failureLevel, isBackground, lifetimeSeconds))
    {
        if (Roster.Pending.Contains(Id) || Roster.Worked.Contains(Id))
            throw new AlreadyExistThreadException(Id.ToString());

        _thread = new Thread(Run)
        {
            Name = Id.ToString(),
            IsBackground = MetaData.IsBackground
        };

        Roster.Pending[Id] = this;
    }

    #region Default life mtods

    protected virtual object? Action()
    {
        return null;
    }

    private void Run()
    {
        try
        {
            // waiting most important thread
            lock (ImportantLock)
            {
                while (ImportantThreadId != null && ImportantThreadId != Id)
                {
                    Monitor.Wait(ImportantLock);
                }
            }

            // move to working threads
            lock (GlobalLock)
            {
                if (!Roster.Pending.Contains(Id))
                    throw new PendingThreadNotExistException(Id.ToString());

                Roster.Pending[Id] = null;
                Roster.Worked[Id] = this;
            }

            using (new Catch(failureLevel: MetaData.FailureLevel, handler: this))
            {
                _actionResult = Action();
            }
        }
        finally
        {
            Release();
        }
    }

    public void Start() => _thread.Start();

    public bool Join(float? timeoutSeconds = null)
    {
        var timeout = timeoutSeconds.HasValue
            ? TimeSpan.FromSeconds(timeoutSeconds.Value)
            : MetaData.ExpiresAt.HasValue
                ? MetaData.ExpiresAt.Value - DateTime.UtcNow
                : Timeout.InfiniteTimeSpan;

        return _thread.Join(timeout);
    }

    public void OnFailure(FailureException failure)
    {
        Console.WriteLine($"Thread {Id} non-critical error: {failure.Message}");
    }

    #endregion

    #region Impotant operations

    public void SetImportant()
    {
        ImportantThreadId = Id;
        while (Roster.Worked.Count > 0)
        {
            Thread.Sleep(10);
        }
    }

    public static void MuteImportant()
    {
        ImportantThreadId = null;
        lock (ImportantLock)
        {
            Monitor.PulseAll(ImportantLock);
        }
    }

    #endregion

    #region Waiting

    public static void WaitAll()
    {
        WaitPending();
        WaitWorked();
    }

    public static void WaitPending()
    {
        while (Roster.Pending.Count > 0)
        {
            Thread.Sleep(10);
        }
    }

    public static void WaitWorked(TimeSpan? timeout = null, Identifier? fromThreadId = null)
    {
        var start = DateTime.UtcNow;

        while (true)
        {
            bool allDone = true;

            foreach (var threadId in Roster.Worked.Keys)
            {
                if (fromThreadId == null || threadId == fromThreadId)
                {
                    allDone = false;
                    break;
                }
            }

            if (allDone) break;

            if (timeout.HasValue && (DateTime.UtcNow - start) > timeout.Value)
                throw new TimeoutException("Thread wait timeout");

            Thread.Sleep(10);
        }
    }

    #endregion

    #region IDisposable Support

    public void Release()
    {
        lock (GlobalLock)
        {
            try
            {
                if (Roster.Pending.Contains(Id))
                    Roster.Pending[Id] = null;

                if (Roster.Worked.Contains(Id))
                    Roster.Worked[Id] = null;
            }
            catch (Exception e)
            {
                throw new ThreadReleaseException(Id.ToString(), e);
            }
        }
    }

    public void Dispose()
    {
        if (_isDisposed) return;

        if (_thread.IsAlive)
        {
            try
            {
                _thread.Interrupt();
            }
            catch
            {
                // ignoring errors
            }
        }

        Release();
        _isDisposed = true;
        GC.SuppressFinalize(this);
    }

    ~QThread() => Dispose();

    #endregion
}