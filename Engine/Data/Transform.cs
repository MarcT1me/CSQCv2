using OpenTK.Mathematics;

namespace Engine.Data;

/// <summary>
/// Данные об ориентации объекта в пространстве
/// </summary>
/// <param name="position">Позиция в трёхмерном пространстве</param>
/// <param name="rotation">Матрица вращения</param>
/// <param name="scale">Размер объекта</param>
public struct Transform(Vector3 position, Quaternion rotation, Vector3 scale)
{
    public Vector3 Position = position;
    public Quaternion Rotation = rotation;
    public Vector3 Size = scale;
}