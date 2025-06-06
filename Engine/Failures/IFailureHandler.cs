namespace Engine.Failures;

public interface IFailureHandler
{
    void OnFailure(FailureException failure);
}