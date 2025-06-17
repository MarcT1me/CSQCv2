namespace Engine.Objects.Scene;

using Data.Meta;
using Data.Collections;
using Events.QuantumEvents;
using SceneNode;

public class Scene(SceneNodeData nodeData)
    : SceneNode<SceneNodeData>(nodeData),
        IEventful, IUpdatable, IRenderable
{
    public static readonly WritableTale<SceneNode<SceneNodeData>> Nodes = new(new MetaData("Scene.Nodes-Table"));

    #region Node methods

    /// <summary>
    /// Добавляет узел в реестр узлов
    /// </summary>
    /// <param name="node">Узел для добавления</param>
    public static void AddNode<T>(SceneNode<T> node) where T : SceneNodeData
    {
        Nodes[node.Id] = node as SceneNode<SceneNodeData>;
    }

    /// <summary>
    /// Выдаёт узел из реестра узлов
    /// </summary>
    /// <param name="identifier">Объект ассоциируемый с узлом</param>
    /// <returns>Узел, если есть</returns>
    public static SceneNode<SceneNodeData>? GetNode(object identifier)
    {
        return Nodes[identifier];
    }

    /// <summary>
    /// Вынимает узел из реестра узлов
    /// </summary>
    /// <param name="identifier">Объект ассоциируемый с узлом</param>
    /// <returns>Узел, если есть</returns>
    public static SceneNode<SceneNodeData>? PopNode(object identifier)
    {
        return Nodes.Pop(identifier);
    }

    public void AddChild<T>(SceneNode<T> node) where T: SceneNodeData
    {
        MetaData.ChildrenIds.Add(node.Id);
        node.MetaData.SceneId = node.Id;
    }

    public override SceneNode<SceneNodeData>? PopChildren(object identifier)
    {
        var node = GetNode(identifier);
        // check Identifier
        if (node is null || MetaData.ChildrenIds.Contains(node.Id)) return null;
        UnlinkParent();

        MetaData.ChildrenIds.Remove(node.Id);
        node.MetaData.SceneId = null;
        return node;
    }

    #endregion


    #region Cycle methods

    public void HandleEvent(QuantumEvent e)
    {
        foreach (var eventfulChild in IterChildren<IEventful>())
        {
            if (eventfulChild.IsActive())
                eventfulChild.HandleEvent(e);
        }
    }

    public void PreUpdate()
    {
        foreach (var updatableChild in IterChildren<IUpdatable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsDynamic())
                updatableChild.PreUpdate();
        }
    }

    public void Update()
    {
        foreach (var updatableChild in IterChildren<IUpdatable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsDynamic())
                updatableChild.Update();
        }
    }

    public void PostUpdate()
    {
        foreach (var updatableChild in IterChildren<IUpdatable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsDynamic())
                updatableChild.PostUpdate();
        }
    }

    public void PreRender()
    {
        foreach (var updatableChild in IterChildren<IRenderable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsVisible())
                updatableChild.PreRender();
        }
    }

    public void Render()
    {
        foreach (var updatableChild in IterChildren<IRenderable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsVisible())
                updatableChild.Render();
        }
    }

    public void PostRender()
    {
        foreach (var updatableChild in IterChildren<IRenderable>())
        {
            if (updatableChild.IsActive() || updatableChild.IsVisible())
                updatableChild.PostRender();
        }
    }

    #endregion
}