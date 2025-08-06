#pragma once

namespace MirageAPI::DirectX::Command
{
    public enum class DX12PrimitiveTopology
    {
        Undefined = 0,
        
        PointList = 1,
        
        LineList = 2,
        LineStrip	= 3,

        TriangleList = 4,
        TriangleStrip = 5,
    };
}
