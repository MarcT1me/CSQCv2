namespace Engine.Data.RegistryManagers;

public interface IRegistryManager<T> where T : class
{
    void Register(T obj);
    T? Get(object id);
    T? Pop(object id);
    int Size { get; }
    ICollection<T> Values { get; }
}