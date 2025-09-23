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
        Sampler = 5,
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

    public enum class DX12ResourceState
    {
        Common = 0,
        VertexAndConstantBuffer = 0x1,
        IndexBuffer = 0x2,
        RenderTarget = 0x4,
        UnorderedAccess = 0x8,

        DepthWrite = 0x10,
        DepthRead = 0x20,

        NonPixelShaderResource = 0x40,
        PixelShaderResource = 0x80,
        AllShaderResource = NonPixelShaderResource | PixelShaderResource,

        StreamOut = 0x100,
        IndirectArgument = 0x200,
        CopyDest = 0x400,
        CopySource = 0x800,
        ResolveDest = 0x1000,
        ResolveSource = 0x2000,
        RaytracingStructure = 0x400000,
        ShadingRateSource = 0x1000000,
        GenericRead = VertexAndConstantBuffer
        | IndexBuffer
        | AllShaderResource
        | IndirectArgument
        | CopySource,

        Present = 0,
        Predication = 0x200,

        VideoDecodeRead = 0x10000,
        VideoDecodeWrite = 0x20000,

        VideoProcessRead = 0x40000,
        VideoProcessWrite = 0x80000,

        VideoEncodeRead = 0x200000,
        VideoEncodeWrite = 0x800000,
    };
}
