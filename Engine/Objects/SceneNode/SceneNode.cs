namespace Engine.Objects.SceneNode;

using Data;
using Data.Meta;
using Scene;

public abstract class SceneNode<T>
    : MetaObject<T>, IDisposable
    where T : SceneNodeData
{
    protected SceneNode(T nodeData) : base(nodeData)
    {
        Scene.AddNode(this); // new Node -> Scene.Nodes
    }

    public ObjectStatusFlags ObjectStatus => MetaData.Status;

    #region Child operation nodes

    /// <summary>
    /// Добавляет узел как ребёнок к текущему
    /// </summary>
    /// <param name="node">Узел-ребёнок</param>
    /// <returns>Узел-ребёнок</returns>
    public virtual SceneNode<T> AddChild(SceneNode<T> node)
    {
        // operate with current node
        MetaData.ChildrenIds.Add(node.Id);
        node.MetaData.ParentId = Id;

        // operate with scene
        if (CurrentScene is { } scene) scene.AddChild(node);

        return node;
    }

    /// <summary>
    /// Выдаёт узла текущего узла
    /// </summary>
    /// <param name="identifier">Объект ассоциируемый с ребёнком</param>
    /// <returns>Узел если есть</returns>
    public virtual SceneNode<SceneNodeData>? GetChildren(object identifier)
    {
        // get Identifier
        var id = Identifier.GiveFromUncertain(identifier);
        // check Identifier
        if (id is null || MetaData.ChildrenIds.Contains(id)) return null;
        // Getting node
        return Scene.GetNode(identifier);
    }

    /// <summary>
    /// Вынимает узел-ребёнка
    /// </summary>
    /// <param name="identifier">Объект ассоциируемый с ребёнком</param>
    /// <returns>Узел если есть</returns>
    public virtual SceneNode<SceneNodeData>? PopChildren(object identifier)
    {
        // get Identifier
        var id = Identifier.GiveFromUncertain(identifier);
        // check Identifier
        if (id is null || MetaData.ChildrenIds.Contains(id)) return null;
        UnlinkParent();

        MetaData.ChildrenIds.Remove(id);

        // update scene MetaData and return node if exist
        return CurrentScene is not { } scene ? null : scene.PopChildren(id);
    }

    /// <summary>
    /// Подключает узел к родителю
    /// </summary>
    /// <param name="parentIdentifier">Объект ассоциируемый с ребёнком</param>
    /// <returns>Родитель, если есть</returns>
    public virtual SceneNode<SceneNodeData>? LinkToParent(Identifier parentIdentifier)
    {
        if (Parent is not null) UnlinkParent();
        if (CurrentScene is null) return null;

        var parent = CurrentScene.GetChildren(parentIdentifier);
        if (parent is null) return null;

        MetaData.ParentId = parentIdentifier;
        return parent;
    }

    /// <summary>
    /// Отключает родителя у узла
    /// </summary>
    public virtual void UnlinkParent()
    {
        if (Parent is null) return;

        Parent.MetaData.ChildrenIds.Remove(Id);
        MetaData.ParentId = null;
    }

    /// <summary>
    /// Итератор для прохода по всем детям текущего узла
    /// </summary>
    /// <returns>Узлы-дети</returns>
    public IEnumerable<SceneNode<SceneNodeData>> IterChildren()
    {
        foreach (var childId in MetaData.ChildrenIds)
        {
            var child = Scene.GetNode(childId);
            if (child != null) yield return child;
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

    /// <summary>
    /// Сцена в которой лежит объект
    /// </summary>
    public Scene? CurrentScene => MetaData.SceneId is not null ? (Scene)Scene.GetNode(MetaData.SceneId)! : null;

    /// <summary>
    /// Родитель узла (если есть)
    /// </summary>
    public SceneNode<SceneNodeData>? Parent => MetaData.ParentId != null ? CurrentScene?.GetChildren(MetaData.ParentId) : null;

    public virtual void Dispose()
    {
        UnlinkParent();
        CurrentScene?.PopChildren(Id);
        Scene.PopNode(Id);
        GC.SuppressFinalize(this);
    }

    ~SceneNode() => Dispose();
}