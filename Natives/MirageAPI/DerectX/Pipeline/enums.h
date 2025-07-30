#pragma once

namespace MirageAPI::DirectX::Pipeline
{
    public enum class DX12PrimitiveTopologyType
    {
        Undefined = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED,
        Point = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
        Line = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
        Triangle = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        Patch = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH
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
