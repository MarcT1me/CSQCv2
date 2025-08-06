namespace Engine.Objects.Scene;

using Data.Meta;
using Data.Collections;
using SceneNode;

public abstract class BaseScene(SceneNodeData nodeData)
    : SceneNode<SceneNodeData>(nodeData)
{
    public static readonly WritableTale<SceneNode<SceneNodeData>> Nodes = new(new MetaData("Scene.Nodes-Table"));

    #region Node methods

    /// <summary>
    /// Добавляет узел в реестр узлов
    /// </summary>
    /// <param name="node">Узел для добавления</param>
    public static void AddNode<T>(SceneNode<T> node) where T : SceneNodeData
    {
        Nodes[node.Id] = node as SceneNode<SceneNodeData>
                         ?? throw new ArgumentException("Node must be of type SceneNode<SceneNodeData>");
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

    /// <summary>
    /// Добавляет узел как ребёнок к текущему
    /// </summary>
    /// <param name="node">ребёнок</param>
    /// <typeparam name="T">тип данных ребёнка</typeparam>
    public void AddChild<T>(SceneNode<T> node) where T : SceneNodeData
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
}