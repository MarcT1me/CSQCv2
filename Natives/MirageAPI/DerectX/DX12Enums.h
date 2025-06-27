#pragma once

#include <d3dcommon.h>

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
}
