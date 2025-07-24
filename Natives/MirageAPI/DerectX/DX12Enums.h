#pragma once

namespace MirageAPI::DirectX
{
    public enum class DX12PrimitiveTopologyType
    {
        Undefined = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED,
        Point = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
        Line = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
        Triangle = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        Patch = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH
    };

    public enum class DX12PrimitiveTopology
    {
        Undefined = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
        PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
        LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST,

        TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        TriangleStrip = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    };

    public enum class DX12TextureType
    {
        None,
        Texture1D,
        Texture2D,
        Texture3D,
        RenderTarget,
        DepthStencil
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

    public enum class DX12ShaderType
    {
        Vertex,
        Pixel,
        Geometry,
        Compute,
        Domain,
        Hull
    };

    [System::Flags]
    public enum class DX12ShaderCompileFlags : unsigned int
    {
        None = 0,
        Debug = D3DCOMPILE_DEBUG,
        SkipOptimization = D3DCOMPILE_SKIP_OPTIMIZATION,
        WarningsAreErrors = D3DCOMPILE_WARNINGS_ARE_ERRORS,
        EnableBackwardsCompatibility = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY,

        OptimizationLevel0 = D3DCOMPILE_OPTIMIZATION_LEVEL0,
        OptimizationLevel1 = D3DCOMPILE_OPTIMIZATION_LEVEL1,
        OptimizationLevel2 = D3DCOMPILE_OPTIMIZATION_LEVEL2,
        OptimizationLevel3 = D3DCOMPILE_OPTIMIZATION_LEVEL3,
    };

    public enum class DX12ShaderVisibility
    {
        Amplification = D3D12_SHADER_VISIBILITY_AMPLIFICATION,
        Vertex = D3D12_SHADER_VISIBILITY_VERTEX,
        Pixel = D3D12_SHADER_VISIBILITY_PIXEL,
        Geometry = D3D12_SHADER_VISIBILITY_GEOMETRY,
        Mesh = D3D12_SHADER_VISIBILITY_MESH,
        Domain = D3D12_SHADER_VISIBILITY_DOMAIN,
        Hull = D3D12_SHADER_VISIBILITY_HULL,
        All = D3D12_SHADER_VISIBILITY_ALL
    };

    public enum class DX12ResourceType
    {
        Constants,
        Buffer,
        ConstantBuffer,
        StructuredBuffer,

        ReadbackBuffer,
        UploadBuffer,

        VertexBuffer,
        IndexBuffer,

        Texture,
        FrameBuffer,
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

    public enum class DX12DescriptorHeapType
    {
        CBV_SRV_UAV,
        Sampler,
        RTV,
        DSV
    };

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

    public enum class DX12BlendOptions
    {
        AlphaFactor = D3D12_BLEND_ALPHA_FACTOR,
        BlendFactor = D3D12_BLEND_BLEND_FACTOR,
        DestAlpha = D3D12_BLEND_DEST_ALPHA,
        DestColor = D3D12_BLEND_DEST_COLOR,
        Src1Alpha = D3D12_BLEND_SRC1_ALPHA,
        Src1Color = D3D12_BLEND_SRC1_COLOR,
        SrcAlpha = D3D12_BLEND_SRC_ALPHA,
        SrcAlphaSat = D3D12_BLEND_SRC_ALPHA_SAT,
        SrcColor = D3D12_BLEND_SRC_COLOR,

        InvAlphaFactor = D3D12_BLEND_INV_ALPHA_FACTOR,
        InvBlendFactor = D3D12_BLEND_INV_BLEND_FACTOR,
        InvDestAlpha = D3D12_BLEND_INV_DEST_ALPHA,
        InvDestColor = D3D12_BLEND_INV_DEST_COLOR,
        InvSrc1Alpha = D3D12_BLEND_INV_SRC1_ALPHA,
        InvSrc1Color = D3D12_BLEND_INV_SRC1_COLOR,
        InvSrcAlpha = D3D12_BLEND_INV_SRC_ALPHA,
        InvSrcColor = D3D12_BLEND_INV_SRC_COLOR,

        OpAdd = D3D12_BLEND_OP_ADD,
        OpSubtract = D3D12_BLEND_OP_SUBTRACT,
        OpMax = D3D12_BLEND_OP_MAX,
        OpMin = D3D12_BLEND_OP_MIN,

        One = D3D12_BLEND_ONE,
        Zero = D3D12_BLEND_ZERO,
    };

    public enum class DX12LogicOperators
    {
        And = D3D12_LOGIC_OP_AND,
        AndInverted = D3D12_LOGIC_OP_AND_INVERTED,
        AndReverse = D3D12_LOGIC_OP_AND_REVERSE,
        Clear = D3D12_LOGIC_OP_CLEAR,
        Copy = D3D12_LOGIC_OP_COPY,
        CopyInverted = D3D12_LOGIC_OP_COPY_INVERTED,
        Equiv = D3D12_LOGIC_OP_EQUIV,
        Invert = D3D12_LOGIC_OP_INVERT,
        Nand = D3D12_LOGIC_OP_NAND,
        Noop = D3D12_LOGIC_OP_NOOP,
        Nor = D3D12_LOGIC_OP_NOR,
        Or = D3D12_LOGIC_OP_OR,
        OrInverted = D3D12_LOGIC_OP_OR_INVERTED,
        OrReverse = D3D12_LOGIC_OP_OR_REVERSE,
        Set = D3D12_LOGIC_OP_SET,
        Xor = D3D12_LOGIC_OP_XOR,
    };

    [System::Flags]
    public enum class DX12ColorWriteEnable : unsigned char
    {
        Red = D3D12_COLOR_WRITE_ENABLE_RED,
        Green = D3D12_COLOR_WRITE_ENABLE_GREEN,
        Blue = D3D12_COLOR_WRITE_ENABLE_BLUE,
        Alpha = D3D12_COLOR_WRITE_ENABLE_ALPHA,
        All = D3D12_COLOR_WRITE_ENABLE_ALL,
    };

    public enum class DX12FillMode
    {
        Solid = D3D12_FILL_MODE_SOLID,
        Wireframe = D3D12_FILL_MODE_WIREFRAME
    };

    public enum class DX12CullMode
    {
        None = D3D12_CULL_MODE_NONE,
        Back = D3D12_CULL_MODE_BACK,
        Front = D3D12_CULL_MODE_FRONT,
    };
}
