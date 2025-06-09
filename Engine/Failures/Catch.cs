namespace Engine.Failures;

using System;
using Data;
using Data.Collections;
using Data.Meta;

/// <summary>
/// Catch с авторской реализацией отлова ошибок
/// </summary>
public sealed class Catch : MetaObject<CatchMeta>, IDisposable
{
    public static readonly WritableTale<Catch> ActiveCatches = new(new MetaData("catchesRoster"));

    public bool IsRunning { get; private set; }

    public bool IsSuccess => MetaData.Failures.IsEmpty(); 

    /// <summary>
    /// Создание Catch
    /// </summary>
    /// <param name="identifier">Объект, ассоциируемый с сетью</param>
    /// <param name="failureLevel">Уровень ошибок по умолчанию</param>
    /// <param name="handler">Обработчик ошибок</param>
    /// <exception cref="InvalidOperationException">Если такой Catch</exception>
    public Catch(
        object? identifier = null,
        FailureLevel failureLevel = FailureLevel.Second,
        IFailureHandler? handler = null
    ) : base(new CatchMeta(identifier, failureLevel, handler))
    {
        if (ActiveCatches[Id] != null)
            throw new InvalidOperationException($"Catch with id {Id} already exists");

        ActiveCatches[Id] = this;
    }

    /// <summary>
    /// Метод для безопасного запуска опасной функции
    /// </summary>
    /// <param name="func">Обрабатываемая функция</param>
    /// <param name="defaultValue">Возвращаемое значение по умолчанию (если случилась ошибка)</param>
    /// <typeparam name="T">Тип возвращаемый из функции</typeparam>
    /// <returns>В удачном случае - результат выполнения оригинальной функции, в ином defaultValue</returns>
    public T? TryFunc<T>(Func<T> func, T? defaultValue = default)
    {
        try
        {
            var temp = IsRunning;
            var ret = func();
            IsRunning = temp;
            return ret;
        }
        catch (Exception ex)
        {
            HandleException(ex);
            return defaultValue;
        }
    }

    public void TryAction(Action action)
    {
        try
        {
            IsRunning = true;
            action();
            IsRunning = false;
        }
        catch (Exception ex)
        {
            HandleException(ex);
        }
    }

    /// <summary>
    /// Обработка ошибок
    /// </summary>
    /// <param name="ex">Обрабатываемая ошибка</param>
    /// <exception cref="FailureException">Если обрабатываемая ошибка отмечена как критичная</exception>
    private void HandleException(Exception ex)
    {
        var failure = ex as FailureException ?? new FailureException(ex.Message, ex)
        {
            Level = MetaData.FailureLevel,
            CatchId = Id
        };
        MetaData.Failures[new Identifier()] = failure;

        if (MetaData.FailureLevel is FailureLevel.First)
        {
            if (MetaData.Handler != null)
            {
                MetaData.Handler.OnFailure(failure);
            }
            else
            {
                EngineCore.DefaultFailureHandler?.OnFailure(failure);
            }
        }
    }

    public void Dispose() => ActiveCatches[Id] = null;

    ~Catch() => Dispose();
}