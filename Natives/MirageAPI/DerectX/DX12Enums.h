#pragma once

#include <d3dcommon.h>
#include <dxgiformat.h>

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
    
    public enum class DX12BufferType
    {
        Vertex,
        Index,
        Constant,
        Structured,
        Readback,
        Upload
    };
}
