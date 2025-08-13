using OpenTK.Mathematics;

namespace Engine.Extensions;

public static class Extensions
{
    public static void ChangeFlag<T>(this ref T obj, T flag, bool isSet) where T : struct, Enum
    {
        dynamic current = obj;
        dynamic flagValue = flag;

        obj = isSet
            ? (T)(current | flagValue)
            : (T)(current & ~flagValue);
    }

    public static void SetYaw(this ref Quaternion q, float yaw)
    {
        var euler = q.ToEulerAngles();
        euler.X = yaw;
        q = Quaternion.FromEulerAngles(euler);
    }

    public static void SetPitch(this ref Quaternion q, float pitch)
    {
        var euler = q.ToEulerAngles();
        euler.Y = pitch;
        q = Quaternion.FromEulerAngles(euler);
    }

    public static void SetRoll(this ref Quaternion q, float roll)
    {
        var euler = q.ToEulerAngles();
        euler.Z = roll;
        q = Quaternion.FromEulerAngles(euler);
    }
}