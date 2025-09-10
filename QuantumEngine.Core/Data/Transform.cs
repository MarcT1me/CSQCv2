using OpenTK.Mathematics;

namespace Engine.Data;

/// <summary>
/// Данные о положении и ориентации объекта в пространстве
/// </summary>
/// <param name="position">Позиция в трёхмерном пространстве</param>
/// <param name="orientation">Матрица вращения</param>
/// <param name="scale">Размер объекта</param>
public struct Transform(Vector3 position, Quaternion orientation, Vector3 scale)
{
    public Vector3 Position = position;
    public Quaternion Orientation = Quaternion.Normalize(orientation);
    public Vector3 Size = scale;

    /// <summary>
    /// Перемещение на какой-то offset
    /// </summary>
    /// <param name="translation">Offset перемещения</param>
    public void Translate(Vector3 translation) =>
        Position += translation;

    /// <summary>
    /// Изменение размера посредством сложение
    /// </summary>
    /// <param name="scale">Добавляемый размер</param>
    public void Scale(Vector3 scale) =>
        Size += scale;

    /// <summary>
    /// Изменение размера посредством умножением
    /// </summary>
    /// <param name="scale">Множитель размера</param>
    public void Scale(float scale) =>
        Size *= scale;

    /// <summary>
    /// Вращение на Quaternion (напрямую)
    /// </summary>
    /// <param name="rotation">Вращение</param>
    public void Rotate(Quaternion rotation) =>
        Orientation = Quaternion.Normalize(rotation * Orientation);

    /// <summary>
    /// Вращение вокруг какого-то вектора на угол
    /// </summary>
    /// <param name="axis">Вектор вращения</param>
    /// <param name="angle">Угол вращения</param>
    public void Rotate(Vector3 axis, float angle) =>
        Orientation = Quaternion.Normalize(Quaternion.FromAxisAngle(axis, angle) * Orientation);

    /// <summary>
    /// Мировая координата, ответственная за "перед": X
    /// </summary>
    public static Vector3 WorldForward => Vector3.UnitX;

    /// <summary>
    /// Мировая координата, ответственная за "право": -Y
    /// </summary>
    public static Vector3 WorldRight => Vector3.UnitY;

    /// <summary>
    /// Мировая координата, ответственная за "верх": Z
    /// </summary>
    public static Vector3 WorldUp => Vector3.UnitZ;

    /// <summary>
    /// Вектор показывающий направление объекта вперёд
    /// </summary>
    public Vector3 Forward => Vector3.Transform(WorldForward, Orientation);

    /// <summary>
    /// Вектор показывающий направление объекта вправо
    /// </summary>
    public Vector3 Right => Vector3.Transform(WorldRight, Orientation);

    /// <summary>
    /// Вектор показывающий направление объекта вверх
    /// </summary>
    public Vector3 Up => Vector3.Transform(WorldUp, Orientation);

    /// <summary>
    /// Обычные углы вращения - Yaw (Up), Pitch (Right), Roll (Forward)
    /// </summary>
    public Vector3 EulerAngles
    {
        get => Orientation.ToEulerAngles();
        set => Orientation = Quaternion.Normalize(Quaternion.FromEulerAngles(value));
    }
}