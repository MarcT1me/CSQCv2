namespace Engine.Extensions;

public static class With
{
    [Obsolete("Not stabel. Use handle method in Catch")]
    public static T Handle<T>(T obj, Action<T> body) where T : IContextManager
    {
        try
        {
            body(obj);
        }
        catch (Exception ex)
        {
            obj.ExceptionHandling(ex);
        }
        finally
        {
            obj.Dispose();
        }
        
        return obj;
    }
}