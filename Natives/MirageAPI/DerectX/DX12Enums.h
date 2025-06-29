#pragma once

#include <d3d12.h>
#include <d3dcommon.h>
#include <dxgiformat.h>
#include <d3dcompiler.h>

namespace MirageAPI::DirectX
{
    public enum class PrimitiveTopology
    {
        Undefined = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
        PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
        LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
        TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        TriangleStrip = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    };

    public enum class DX12TextureType
    {
        Texture2D,
        RenderTarget,
        DepthStencil,
        CubeMap
    };

    public enum class DX12TextureFormat
    {
        RGBA8_UNORM = DXGI_FORMAT_R8G8B8A8_UNORM,
        RGBA16_FLOAT = DXGI_FORMAT_R16G16B16A16_FLOAT,
        D32_FLOAT = DXGI_FORMAT_D32_FLOAT,
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

    public enum class DX12ShaderCompileFlags : UINT
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

    public enum class DX12BufferType
    {
        Vertex,
        Index,
        Texture,
        Frame,

        Constant,
        Structured,
        Readback,
        Upload
    };

    public enum class DX12CommandListType
    {
        Direct = D3D12_COMMAND_LIST_TYPE_DIRECT,
        Compute = D3D12_COMMAND_LIST_TYPE_COMPUTE,
        Bundle = D3D12_COMMAND_LIST_TYPE_BUNDLE,
        Copy = D3D12_COMMAND_LIST_TYPE_COPY,

        VideoEncode = D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE,
        VideoProcess = D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS,
        VideoDecode = D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE,
    };
}
