namespace Engine.Extensions;

public static class With
{
    public static void Handle<T>(T obj, Action<T> body) where T : IContextManager
    {
        try
        {
            body(obj);
        }
        catch (Exception ex)
        {
            obj.OnException(ex);
        }
        finally
        {
            obj.Dispose();
        }
    }
}