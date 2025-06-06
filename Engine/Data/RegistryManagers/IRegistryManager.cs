namespace Engine.Data.RegistryManagers;

public interface IRegistryManager<T> where T : class
{
    public void Register(T obj);
    public T? Get(object id);
}