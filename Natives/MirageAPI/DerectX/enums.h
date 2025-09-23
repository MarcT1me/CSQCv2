#pragma once

namespace MirageAPI::DirectX
{
    public enum class DX12ResourceType : UINT8
    {
        Texture = 0,
        RenderTarget = 0,

        Constants = 1,

        Buffer,
        ConstantBuffer = 2,
        StructuredBuffer = 3,
        ReadbackBuffer,
        UploadBuffer,
        VertexBuffer,
        IndexBuffer,
    };

    public enum class DX12ResourceFormat
    {
        Unknown = DXGI_FORMAT_UNKNOWN,

        // 1 байт на пиксель
        R8_SINT = DXGI_FORMAT_R8_SINT,
        R8_UINT = DXGI_FORMAT_R8_UINT,
        R8_SNORM = DXGI_FORMAT_R8_SNORM,
        R8_UNORM = DXGI_FORMAT_R8_UNORM,

        // 2 байта на пиксель
        R16_UINT = DXGI_FORMAT_R16_UINT,
        RG8_SINT = DXGI_FORMAT_R8G8_SINT,
        RG8_UINT = DXGI_FORMAT_R8G8_UINT,
        RG8_SNORM = DXGI_FORMAT_R8G8_SNORM,
        RG8_UNORM = DXGI_FORMAT_R8G8_UNORM,
        BGRA4_UNORM = DXGI_FORMAT_B4G4R4A4_UNORM,
        R16_FLOAT = DXGI_FORMAT_R16_FLOAT,

        // 4 байта на пиксель
        R32_UINT = DXGI_FORMAT_R32_UINT,
        RGBA8_SINT = DXGI_FORMAT_R8G8B8A8_SINT,
        RGBA8_UINT = DXGI_FORMAT_R8G8B8A8_UINT,
        RGBA8_SNORM = DXGI_FORMAT_R8G8B8A8_SNORM,
        RGBA8_UNORM = DXGI_FORMAT_R8G8B8A8_UNORM,
        RGBA8_UNORM_SRGB = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        BGRA8_UNORM = DXGI_FORMAT_B8G8R8A8_UNORM,
        BGRA8_UNORM_SRGB = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
        R32_FLOAT = DXGI_FORMAT_R32_FLOAT,
        RG16_FLOAT = DXGI_FORMAT_R16G16_FLOAT,

        // 8 байт на пиксель
        RGBA16_SINT = DXGI_FORMAT_R16G16B16A16_SINT,
        RGBA16_UINT = DXGI_FORMAT_R16G16B16A16_UINT,
        RGBA16_SNORM = DXGI_FORMAT_R16G16B16A16_SNORM,
        RGBA16_UNORM = DXGI_FORMAT_R16G16B16A16_UNORM,
        RG32_FLOAT = DXGI_FORMAT_R32G32_FLOAT,
        RGBA16_FLOAT = DXGI_FORMAT_R16G16B16A16_FLOAT,

        // 12 байт на пиксель
        RGB32_SINT = DXGI_FORMAT_R32G32B32_SINT,
        RGB32_UINT = DXGI_FORMAT_R32G32B32_UINT,
        RGB32_FLOAT = DXGI_FORMAT_R32G32B32_FLOAT,

        // 16 байт на пиксель
        RGBA32_FLOAT = DXGI_FORMAT_R32G32B32A32_FLOAT
    };
    [System::Flags]
    public enum class DX12DeviceInitFlags
    {
        None = 0,
        Debug = 1,
        UseAdapter = 2,
        UseWarpAdapter = UseAdapter | 4,
        UseHighPerformanceAdapter = UseAdapter | 8
    };
}
