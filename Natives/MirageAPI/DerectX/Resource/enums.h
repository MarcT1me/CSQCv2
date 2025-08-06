#pragma once

namespace MirageAPI::DirectX::Resource
{
    public enum class DX12ViewDimension
    {
        Unknown = 0,
        Buffer = 1,

        Texture1D = 2,
        Texture1DArray = 3,

        Texture2D = 4,
        Texture2DArray = 5,
        Texture2DMS = 6,
        Texture2DMSAray = 7,

        Texture3D = 8,
        TextureCube = 9,
        TextureCubeArray = 10,

        RaytracingStructure = 11,
    };

    public enum class DX12TextureType
    {
        Unknown = 0,
        Buffer = 1,

        Texture1D = 2,

        Texture2D = 3,
        RenderTarget = Texture2D,
        DepthStencil = Texture2D,

        Texture3D = 4,
    };

    [System::Flags]
    public enum class DX12ResourceFlags
    {
        None = 0,
        AllowRenderTarget = 0x1,
        AllowDepthStencil = 0x2,
        AllowUnorderedAccess = 0x4,
        DenyShaderResource = 0x8,
        AllowCrossAdapter = 0x10,
        AllowSimultaneousAccess = 0x20,
        VideoDecodeReferenceOnly = 0x40,
        VideoEncodeReferenceOnly = 0x80,
        RaytracingStructure = 0x100
    };
}
