namespace Engine.Threading;

using Data;
using Data.Meta;
using Failures;
using Logging;

/// <summary>
/// Класс потоков с авторской реализацией, внедрённый в системы движка
/// </summary>
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

    /// <summary>
    /// Создание потока
    /// </summary>
    /// <param name="name">Имя (в последствии Identifier потока)</param>
    /// <param name="failureLevel">Уровень ошибок, обрабатываемый в потоках</param>
    /// <param name="isBackground">Позволяет потоку работать после прекращения главного потока</param>
    /// <param name="lifetimeSeconds">Время жизни потока с начала работы</param>
    /// <exception cref="AlreadyExistThreadException">Если такой поток уже существует</exception>
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

    /// <summary>
    /// Метод, запускаемый в другом потоке
    /// </summary>
    /// <returns>Результат его выполнения</returns>
    protected virtual object? Action()
    {
        return null;
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
            $"Thread {Id} catch got {failure.Level} level error:\n" +
            $"{failure.GetType()}: {failure.Message}"
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

    ~QThread() => Dispose();

    #endregion
}