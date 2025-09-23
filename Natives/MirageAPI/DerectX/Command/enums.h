#pragma once

namespace MirageAPI::DirectX::Command
{
    public enum class DX12PrimitiveTopology
    {
        Undefined = 0,

        PointList = 1,

        LineList = 2,
        LineStrip = 3,

        TriangleList = 4,
        TriangleStrip = 5,
    };

    public enum class DX12CommandListType
    {
        Direct = 0,
        Bundle = 1,
        Compute = 2,
        Copy = 3,

        VideoDecode = 4,
        VideoProcess = 5,
        VideoEncode = 6
    };
}
