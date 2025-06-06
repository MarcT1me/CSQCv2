namespace Engine.Failures;

using System;
using Data;
using Data.Arrays;
using Data.Meta;

public sealed class Catch : MetaObject<CatchMeta>, IDisposable
{
    private static readonly WritableTale<Catch> ActiveCatches = new(new MetaData("catchesRoster"));

    public bool IsRunning { get; private set; }

    public bool IsSuccess => MetaData.Failures.IsEmpty(); 

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

        if (MetaData.FailureLevel is FailureLevel.Second)
        {
            throw failure;
        }
    }

    public void Dispose() => ActiveCatches[Id] = null;

    ~Catch() => Dispose();
}