namespace QuantumCore.Failures;

using Data.Meta;
using Data.Collections;

/// <summary>
/// Контейнер для хранения ошибок
/// </summary>
/// <param name="metaData"></param>
public sealed class FailuresRoster(MetaData metaData) : Roster<FailureException>(metaData)
{
    /// <summary>
    /// Check Exception type availability in roster 
    /// </summary>
    /// <typeparam name="T">Type of checked Exception</typeparam>
    /// <returns>true if type is exist in roster</returns>
    public bool Contains<T>() where T : Exception
    {
        Type exceptionType = typeof(T);

        foreach (FailureException failure in Values)
        {
            if (failure.GetType() == exceptionType || failure.InnerException?.GetType() == exceptionType)
                return true;
        }

        return false;
    }
}