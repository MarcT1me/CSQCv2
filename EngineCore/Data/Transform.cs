using OpenTK.Mathematics;

namespace Engine.Data;

/// <summary>
/// Данные об ориентации объекта в пространстве
/// </summary>
/// <param name="position">Позиция в трёхмерном пространстве</param>
/// <param name="rotation">Матрица вращения</param>
/// <param name="scale">Размер объекта</param>
public struct Transform(Vector3 position, Vector3 rotation, Vector3 scale)
{
    public Vector3 Position = position;
    public Vector3 Rotation = rotation;
    public Vector3 Size = scale;

    public static Vector3 UpVector => Vector3.UnitY;
    public static Vector3 RightVector => Vector3.UnitX;
    public static Vector3 ForwardVector => -Vector3.UnitZ;
}