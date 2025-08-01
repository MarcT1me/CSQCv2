#pragma once

namespace MirageAPI::DirectX
{
    public enum class DX12HeapType
    {
        Custom = D3D12_HEAP_TYPE_CUSTOM,
        Default = D3D12_HEAP_TYPE_DEFAULT,
        Readback = D3D12_HEAP_TYPE_READBACK,
        Upload = D3D12_HEAP_TYPE_UPLOAD,
    };

    public enum class DX12CommandListType
    {
        Bundle = D3D12_COMMAND_LIST_TYPE_BUNDLE,
        Compute = D3D12_COMMAND_LIST_TYPE_COMPUTE,
        Copy = D3D12_COMMAND_LIST_TYPE_COPY,
        Direct = D3D12_COMMAND_LIST_TYPE_DIRECT,

        VideoDecode = D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE,
        VideoEncode = D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE,
        VideoProcess = D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS,
    };

    public enum class DX12DescriptorHeapType
    {
        CBV_SRV_UAV,
        Sampler,
        RTV,
        DSV
    };

    public enum class DX12ResourceType
    {
        Constants = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
        
        Buffer,
        ConstantBuffer = D3D12_ROOT_PARAMETER_TYPE_CBV,
        StructuredBuffer = D3D12_ROOT_PARAMETER_TYPE_SRV,
        ReadbackBuffer,
        UploadBuffer,
        VertexBuffer,
        IndexBuffer,

        Texture = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        FrameBuffer = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
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

    public enum class DX12ResourceState
    {
        Common = D3D12_RESOURCE_STATE_COMMON,

        CopyDest = D3D12_RESOURCE_STATE_COPY_DEST,
        CopySource = D3D12_RESOURCE_STATE_COPY_SOURCE,

        DepthRead = D3D12_RESOURCE_STATE_DEPTH_READ,
        DepthWrite = D3D12_RESOURCE_STATE_DEPTH_WRITE,

        GenericRead = D3D12_RESOURCE_STATE_GENERIC_READ,
        IndirectArgument = D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,

        IndexBuffer = D3D12_RESOURCE_STATE_INDEX_BUFFER,
        VertexAndConstantBuffer = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,

        AllShaderResources = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE,
        PixelShaderRes = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        NonPixelShaderRes = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,

        Predication = D3D12_RESOURCE_STATE_PREDICATION,
        Present = D3D12_RESOURCE_STATE_PRESENT,

        RayTracingAccelerationStructure = D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
        RenderTarget = D3D12_RESOURCE_STATE_RENDER_TARGET,

        ResolveDest = D3D12_RESOURCE_STATE_RESOLVE_DEST,
        ResolveSource = D3D12_RESOURCE_STATE_RESOLVE_SOURCE,

        ShadingRateSource = D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE,
        StreamOut = D3D12_RESOURCE_STATE_STREAM_OUT,
        UnorderedAccess = D3D12_RESOURCE_STATE_UNORDERED_ACCESS,

        VideoDecodeRead = D3D12_RESOURCE_STATE_VIDEO_DECODE_READ,
        VideoDecodeWrite = D3D12_RESOURCE_STATE_VIDEO_DECODE_WRITE,
        VideoEncodeRead = D3D12_RESOURCE_STATE_VIDEO_ENCODE_READ,
        VideoEncodeWrite = D3D12_RESOURCE_STATE_VIDEO_ENCODE_WRITE,
        VideoProcessRead = D3D12_RESOURCE_STATE_VIDEO_PROCESS_READ,
        VideoProcessWrite = D3D12_RESOURCE_STATE_VIDEO_PROCESS_WRITE,
    };
    
    [System::Flags]
    public enum class DX12DeviceInitFlags : unsigned int
    {
        None = 0,
        Debug = 1,
        UseAdapter = 2,
        UseWarpAdapter = 4,
        UseHighPerformanceAdapter = 8
    };
}
