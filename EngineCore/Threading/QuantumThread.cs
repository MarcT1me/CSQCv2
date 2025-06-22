namespace Engine.Threading;

using Data;
using Data.Meta;
using Failures;
using Logging;
using Extensions;

/// <summary>
/// Класс потоков с авторской реализацией, внедрённый в системы движка
/// </summary>
public class QuantumThread : MetaObject<QThreadMeta>, IDisposable, IFailureHandler
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

    public static QuantumThread? Current
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
    private bool _isDisposed;

    public bool IsAlive => _thread.IsAlive;
    public ThreadState State => _thread.ThreadState;

    private readonly Action? _action;

    #endregion

    /// <summary>
    /// Создание потока
    /// </summary>
    /// <param name="name">Имя (в последствии Identifier потока)</param>
    /// <param name="failureLevel">Уровень ошибок, обрабатываемый в потоках</param>
    /// <param name="isBackground">Позволяет потоку работать после прекращения главного потока</param>
    /// <param name="lifetimeSeconds">Время жизни потока с начала работы</param>
    /// <param name="action">Действие, для быстрого создания потока</param>
    /// <exception cref="AlreadyExistThreadException">Если такой поток уже существует</exception>
    public QuantumThread(
        string? name = null,
        FailureLevel? failureLevel = null,
        bool isBackground = true,
        float? lifetimeSeconds = null,
        Action? action = null
    ) : base(new QThreadMeta(name, failureLevel, isBackground, lifetimeSeconds))
    {
        if (Roster.Pending.Contains(Id) || Roster.Worked.Contains(Id))
            throw new AlreadyExistThreadException(Id.ToString());

        _thread = new Thread(Run)
        {
            Name = Id.ToString(),
            IsBackground = MetaData.IsBackground
        };
        _action = action;

        Roster.Pending[Id] = this;
    }

    #region Default life mtods

    /// <summary>
    /// Метод, запускаемый в другом потоке
    /// </summary>
    /// <returns>Результат его выполнения</returns>
    protected virtual void Action()
    {
        _action?.Invoke();
    }

    /// <summary>
    /// Запуск действий в новом потоке
    /// </summary>
    public void Start() => _thread.Start(() => Run());

    /// <summary>
    /// Просто запуск действий
    /// </summary>
    /// <exception cref="PendingThreadNotExistException">Если поток был потерян во время выполнения</exception>
    /// <remarks>Происходит в том же потоке</remarks>
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

            // update lifetime after waiting
            MetaData.RessetLifetime();

            // move to working threads
            lock (GlobalLock)
            {
                var t = Roster.Pending.Pop(Id);
                if (t == null)
                    throw new PendingThreadNotExistException(Id.ToString());
                Roster.Worked[Id] = t;
            }

            With.Handle(
                new Catch(failureLevel: MetaData.FailureLevel, handler: this),
                _ => Action()
            );
        }
        finally
        {
            Release();
        }
    }

    /// <summary>
    /// Присоединение потока к текущему
    /// </summary>
    /// <param name="timeoutSeconds">Максимальное время ожидания</param>
    /// <returns>Успешность действия</returns>
    public bool Join(float? timeoutSeconds = null)
    {
        var timeout = timeoutSeconds.HasValue
            ? TimeSpan.FromSeconds(timeoutSeconds.Value)
            : MetaData.ExpiresAt.HasValue
                ? MetaData.ExpiresAt.Value - DateTime.UtcNow
                : Timeout.InfiniteTimeSpan;

        return _thread.Join(timeout);
    }

    /// <summary>
    /// Реализация обработки ошибок
    /// </summary>
    /// <param name="failure"></param>
    public void OnFailure(FailureException failure)
    {
        Logger.Warning(
            $"Thread '{Id}' catch got '{failure.Level}' level error:\n" +
            $"{failure.GetType().Name}: {failure.Message}"
        );

        failure.Handle();
    }

    #endregion

    #region Impotant operations

    /// <summary>
    /// Установка потока, как важного
    /// </summary>
    public void SetImportant()
    {
        ImportantThreadId = Id;
        while (Roster.Worked.Count > 0)
        {
            Thread.Sleep(10);
        }
    }

    /// <summary>
    /// Обнуление важного потока
    /// </summary>
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

    /// <summary>
    /// Ожидание выполнения всех запланированных потоков
    /// </summary>
    public static void WaitAll()
    {
        WaitPending();
        WaitWorked();
    }

    /// <summary>
    /// Ожидания ждущих потоков
    /// </summary>
    public static void WaitPending()
    {
        while (Roster.Pending.Count > 0)
        {
            Thread.Sleep(10);
        }
    }

    /// <summary>
    /// Ожидание работающих потоков
    /// </summary>
    /// <param name="timeout">Максимальное время ожидания</param>
    /// <param name="fromThreadId">Поток, вызвавший функцию</param>
    /// <exception cref="TimeoutException">Если время вышло, а потоки всё ещё работают</exception>
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

    /// <summary>
    /// Предварительная работа по удалению потока
    /// </summary>
    /// <exception cref="ThreadReleaseException">Если в результате удаления потока из контейнера произошёл сбой</exception>
    public void Release()
    {
        lock (GlobalLock)
        {
            try
            {
                if (Roster.Pending.Contains(Id))
                    Roster.Pending.Pop(Id);

                if (Roster.Worked.Contains(Id))
                    Roster.Worked.Pop(Id);
            }
            catch (Exception e)
            {
                throw new ThreadReleaseException(Id.ToString(), e);
            }
        }
    }

    /// <summary>
    /// Очистка потока из памяти
    /// </summary>
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

    ~QuantumThread() => Dispose();

    #endregion
}