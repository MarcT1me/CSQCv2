using System.Reflection;
using Engine.Logging;

namespace Engine.Failures;

using System;
using Data;
using Data.Collections;
using Data.Meta;
using Extensions;

/// <summary>
/// Catch с авторской реализацией отлова ошибок
/// </summary>
public sealed class Catch : MetaObject<CatchMeta>, IContextManager
{
    /// <summary>
    /// Реестр всех существующих в данный момент Catch
    /// </summary>
    public static readonly WritableTale<Catch> ActiveCatches = new(new MetaData("catchesRoster"));

    private readonly List<Action<Catch>> _tryActions = new(); // список try {}
    private readonly Dictionary<Type, Func<Catch, Exception, bool>> _exceptHandlers = new(); // список except (T ex) {}
    private readonly List<Action<Catch>> _finallyActions = new(); // список finally {}

    /// <summary>
    /// Показатель работы Catch, равен true при запуске Handle
    /// </summary>
    public bool IsRunning => MetaData.IsRunning;

    /// <summary>
    /// Показатель успешности выполнения, равен false при отлове хотя-бы одной ошибки
    /// </summary>
    public bool IsSuccess => MetaData.IsSuccess;

    /// <summary>
    /// Показатель обработанности выводов, равен true в случае если не было отловлено ни одной ошибки,
    /// или отловленная ошибка была успешно обработана
    /// </summary>
    public bool IsHandled => MetaData.IsHandled;

    /// <summary>
    /// Улучшенный обработчик ошибок (try-except)
    /// </summary>
    /// <param name="identifier">Объект, ассоциируемый с обработчиком</param>
    /// <param name="failureLevel">Уровень ошибок по умолчанию</param>
    /// <param name="handler">Обработчик ошибок, если нужен</param>
    /// <exception cref="InvalidOperationException">Если Catch с таким именем уже зарегистрирован в реестре</exception>
    public Catch(
        object? identifier = null,
        FailureLevel failureLevel = FailureLevel.Second,
        IFailureHandler? handler = null
    ) : base(new CatchMeta(identifier, failureLevel, handler))
    {
        ActiveCatches[Id] = this;
        MetaData.IsRunning = true;
    }

    /// <summary>
    /// Метод для "безопасного запуска" функции
    /// </summary>
    /// <param name="action">Обрабатываемая функция (принимающая текущий Catch на вход)</param>
    public void TryAction(Action<Catch> action)
    {
        // just DRY
        TryAction(() => action(this));
    }

    /// <summary>
    /// Метод для "безопасного запуска" функции
    /// </summary>
    /// <param name="action">Обрабатываемая функция</param>
    public void TryAction(Action action)
    {
        // try execute
        try
        {
            action();
        }
        catch (Exception ex)
        {
            // catch exception and handle him (without calling intricate except and finally methods)
            ExceptionHandling(ex);
        }
    }

    /// <summary>
    /// Блок `try`
    /// </summary>
    /// <param name="action">Действие для обработки</param>
    /// <returns>Catch для продолжения работы</returns>
    /// <remarks>Может быть вызван многократно, обработаны будут все вызовы.
    /// В случае возникновения ошибки цепочка вызовов прекратится</remarks>
    public Catch Try(Action<Catch> action)
    {
        _tryActions.Add(action);
        return this;
    }

    /// <summary>
    /// Блок `except (T ex)`
    /// </summary>
    /// <param name="handler">Обработчик ошибки</param>
    /// <typeparam name="T">Тип отлавливаемой ошибки</typeparam>
    /// <returns>Catch для продолжения работы</returns>
    public Catch Except<T>(Action<Catch, Exception> handler) where T : Exception
    {
        _exceptHandlers.Add(typeof(T), (cth, ex) =>
        {
            handler(cth, ex);
            return false;
        });
        return this;
    }

    /// <summary>
    /// Блок `except (T ex)`
    /// </summary>
    /// <param name="handler">Обработчик ошибки</param>
    /// <typeparam name="T">Тип отлавливаемой ошибки</typeparam>
    /// <returns>Catch для продолжения работы</returns>
    /// <remarks>Может быть конечным обработчиком, если при выполнении вернуть true</remarks>
    public Catch Except<T>(Func<Catch, Exception, bool> handler) where T : Exception
    {
        _exceptHandlers.Add(typeof(T), handler);
        return this;
    }

    /// <summary>
    /// Блок `finally`
    /// </summary>
    /// <param name="action">Вызываемая функция</param>
    /// <returns>Catch для продолжения работы</returns>
    /// <remarks>Может быть вызван несколько раз, будут вызваны все по очереди</remarks>
    public Catch Finally(Action<Catch> action)
    {
        _finallyActions.Add(action);
        return this;
    }

    /// <summary>
    /// Запуск обработки
    /// </summary>
    /// <returns>Cath дя продолжения работы над ним</returns>
    public Catch Handle()
    {
        // set Running flag
        MetaData.IsRunning = true;

        // simple try-except
        try
        {
            // iterate all actions
            foreach (var action in _tryActions) action(this);
            // set Handled flag if success
            MetaData.IsHandled = true;
        }
        catch (Exception ex)
        {
            // iterate and execute handlers
            foreach (var (type, exceptionHandler) in _exceptHandlers)
            {
                if (!type.IsInstanceOfType(ex)) continue; // filter
                MetaData.IsHandled |= exceptionHandler(this, ex); // handle
                if (IsHandled) break; // stop operations if is final handling
            }

            // if handlers haven't finished their work
            if (!IsHandled) ExceptionHandling(ex);
        }
        finally
        {
            // iterate all finally actions
            foreach (var finallyAction in _finallyActions) finallyAction(this);
        }

        // We are no longer working - set running flag as false
        MetaData.IsRunning = false;
        return this;
    }

    /// <summary>
    /// Обработка ошибок
    /// </summary>
    /// <param name="ex">Обрабатываемая ошибка</param>
    /// <exception cref="FailureException">Если обрабатываемая ошибка отмечена как критичная</exception>
    public void ExceptionHandling(Exception ex)
    {
        // FUCKING TargetInvocationException<> 
        if (ex is TargetInvocationException && ex.InnerException != null) ex = ex.InnerException;

        // BEEEP!!! ERRORRRR
        Console.Beep();
        Logger.Warning(
            $"Catch with id '{Id}' got '{MetaData.FailureLevel}' level error:\n" +
            $"{ex.Message}"
        );

        // creating failure if need
        var failure = ex as FailureException ?? new FailureException(ex.Message, ex)
        {
            Level = MetaData.FailureLevel,
            CatchId = Id
        };
        failure.CatchId ??= Id;
        MetaData.Failures[new Identifier()] = failure; // register him in MetaData

        // filter NotCritical errors
        if (MetaData.FailureLevel is FailureLevel.NotCritical) return;

        // handling
        if (MetaData.Handler != null)
            MetaData.Handler.OnFailure(failure);
        else
            EngineCore.DefaultFailureHandler.OnFailure(failure);
    }

    public void Dispose()
    {
        MetaData.IsRunning = false;
        ActiveCatches.Pop(Id);
        GC.SuppressFinalize(this);
    }

    ~Catch() => Dispose();
}