namespace Engine.Failures;

using Data.Meta;
using Data.Collections;

public sealed class FailuresRoster(MetaData metaData) : Roster<FailureException>(metaData)
{
    public bool Contains(Type exceptionType)
    {
        foreach (var failure in Values)
        {
            if (failure.GetType() == exceptionType || failure.InnerException?.GetType() == exceptionType)
                return true;
        }

        return false;
    }
}