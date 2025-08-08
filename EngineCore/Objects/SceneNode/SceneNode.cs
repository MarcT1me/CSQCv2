namespace Engine.Objects.SceneNode;

using Data.Meta;

public abstract class SceneNode<TData>
    : MetaObject<TData>, IDisposable
    where TData : SceneNodeData
{
    public ObjectStatusFlags ObjectStatus => MetaData.Status;

    protected SceneNode(TData nodeData) : base(nodeData)
    {
        Data.RegistryManagers.CoreRegistries.NodeRegistry.Register(Id, this);
    }

    #region Child operation nodes

    /// <summary>
    /// Добавляет узел как ребёнок к текущему
    /// </summary>
    /// <param name="identifier">Объект ассоциируемый с ребёнком</param>
    /// <returns>Узел-ребёнок</returns>
    public virtual SceneNode<T> AddChild<T>(object identifier) where T : SceneNodeData
    {
        var node = Data.RegistryManagers.CoreRegistries.NodeRegistry.Get(identifier) as SceneNode<T>;
        if (node is null) return null!;

        // operate with current node
        MetaData.ChildrenIds.Add(node.Id);
        node.MetaData.ParentId = Id;
        node.MetaData.SceneId = MetaData.SceneId;

        return node;
    }

    /// <summary>
    /// Добавляет узел как ребёнок к текущему
    /// </summary>
    /// <param name="node">Ребёнок</param>
    /// <returns>Узел-ребёнок</returns>
    public virtual SceneNode<T> AddChild<T>(SceneNode<T> node) where T : SceneNodeData
    {
        // operate with current node
        MetaData.ChildrenIds.Add(node.Id);
        node.MetaData.ParentId = Id;
        node.MetaData.SceneId = MetaData.SceneId;

        return node;
    }

    /// <summary>
    /// Выдаёт узла текущего узла
    /// </summary>
    /// <param name="identifier">Объект ассоциируемый с ребёнком</param>
    /// <returns>Узел если есть</returns>
    public virtual SceneNode<T>? GetChildren<T>(object identifier) where T : SceneNodeData
    {
        // get node and check children
        var node = Data.RegistryManagers.CoreRegistries.NodeRegistry.Get(identifier) as SceneNode<T>;
        if (node is null || MetaData.ChildrenIds.Contains(node.Id)) return null;

        return node;
    }

    /// <summary>
    /// Вынимает узел-ребёнка
    /// </summary>
    /// <param name="identifier">Объект ассоциируемый с ребёнком</param>
    /// <returns>Узел если есть</returns>
    public virtual SceneNode<T>? PopChildren<T>(object identifier) where T : SceneNodeData
    {
        // get node and check children
        var node = Data.RegistryManagers.CoreRegistries.NodeRegistry.Get(identifier) as SceneNode<T>;
        if (node is null || MetaData.ChildrenIds.Contains(node.Id)) return null;

        UnlinkParent<T>();
        return node;
    }

    /// <summary>
    /// Подключает узел к родителю
    /// </summary>
    /// <param name="identifier">Объект ассоциируемый с ребёнком</param>
    /// <returns>Родитель, если есть</returns>
    public virtual object? LinkToParent<T>(object identifier) where T : SceneNodeData
    {
        var node = Data.RegistryManagers.CoreRegistries.NodeRegistry.Get(identifier) as SceneNode<T>;
        if (node is null) return null;

        if (MetaData.ParentId is not null) UnlinkParent<T>();

        node.MetaData.ChildrenIds.Add(Id);
        MetaData.ParentId = node.Id;
        MetaData.SceneId = node.MetaData.SceneId;
        return node;
    }

    public virtual SceneNode<T>? GetParent<T>() where T : SceneNodeData
    {
        if (MetaData.ParentId is null) return null;
        return Data.RegistryManagers.CoreRegistries.NodeRegistry.Get(MetaData.ParentId) as SceneNode<T>;
    }

    /// <summary>
    /// Отключает родителя у узла
    /// </summary>
    public virtual void UnlinkParent<T>() where T : SceneNodeData
    {
        if (MetaData.ParentId is null) return;

        var node = Data.RegistryManagers.CoreRegistries.NodeRegistry.Get(MetaData.ParentId) as SceneNode<T>;
        
        if (node is null) return;

        node.MetaData.ChildrenIds.Remove(Id);
        MetaData.ParentId = null;
        MetaData.SceneId = null;
    }

    /// <summary>
    /// Итератор для прохода по всем детям текущего узла
    /// </summary>
    /// <returns>Узлы-дети</returns>
    public IEnumerable<object> IterChildren()
    {
        foreach (var childId in MetaData.ChildrenIds)
        {
            var child = Data.RegistryManagers.CoreRegistries.NodeRegistry.Get(childId)!;
            yield return child;
        }
    }

    /// <summary>
    /// Итератор по всем детям-узлам, подходящим под тип
    /// </summary>
    /// <typeparam name="TFilter">Тип - фильтр</typeparam>
    /// <returns>Дети, прошедшую фильтрацию по типу</returns>
    public IEnumerable<TFilter> IterChildren<TFilter>()
    {
        foreach (var child in IterChildren())
        {
            if (child is TFilter t) yield return t;
        }
    }

    #endregion

    public virtual void Dispose()
    {
        Data.RegistryManagers.CoreRegistries.NodeRegistry.Pop(Id);
        GC.SuppressFinalize(this);
    }

    ~SceneNode() => Dispose();
}