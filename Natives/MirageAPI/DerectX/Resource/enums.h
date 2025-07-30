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
        Texture1D = D3D12_RESOURCE_DIMENSION_TEXTURE1D,
        Texture2D = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        RenderTarget = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        DepthStencil = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        Texture3D = D3D12_RESOURCE_DIMENSION_TEXTURE3D,
    };

    [System::Flags]
    public enum class DX12ResourceFlags : unsigned int
    {
        None = D3D12_RESOURCE_FLAG_NONE,

        AllowCrossAdapter = D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER,
        AllowDepthStencil = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
        AllowRenderTarget = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
        AllowSimultaneousAccess = D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS,
        AllowUnorderedAccess = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,

        DenyShaderResource = D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE,
        RaytracingAccelerationStructure = D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE,

        VideoDecodeReferenceOnly = D3D12_RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY,
        VideoEncodeReferenceOnly = D3D12_RESOURCE_FLAG_VIDEO_ENCODE_REFERENCE_ONLY,
    };
}
