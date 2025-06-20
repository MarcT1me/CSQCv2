namespace Engine.Failures;

using Data.Meta;
using Data.Collections;

/// <summary>
/// Контейнер для хранения ошибок
/// </summary>
/// <param name="metaData"></param>
public sealed class FailuresRoster(MetaData metaData) : Roster<FailureException>(metaData)
{
    public bool Contains(Type exceptionType)
    {
        foreach (FailureException failure in Values)
        {
            if (failure.GetType() == exceptionType || failure.InnerException?.GetType() == exceptionType)
                return true;
        }

        return false;
    }
}