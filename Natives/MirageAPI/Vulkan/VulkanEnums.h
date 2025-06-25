#pragma once

namespace MirageAPI::Vulkan
{
    public enum class PixelFormat
    {
        RGBA_SRGB,
        BGRA_SRGB,
        RGBA_FLOAT,
    };

    public enum class PipelineType
    {
        Texture,
        SolidColor,
        RTX
    };

    public value struct ClearColor
    {
        float R;
        float G;
        float B;
        float A;
    };
}