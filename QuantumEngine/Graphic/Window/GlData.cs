using OpenTK.Mathematics;
using MirageAPI;

namespace Engine.Graphic.Window;

public sealed class GlData
{
    // Static init info
    public static int NumberOfSamples = 8;
    public static int DepthBits = 24;

    // Дополнительные настройки Vulkan
    public static bool EnableValidationLayers = true; // Включить слои валидации
    public static int MaxFramesInFlight = 2; // Количество кадров "в полете"

    // other
    public Vector4 ClearColor = new(0.08f, 0.16f, 0.18f, 1.0f);
}