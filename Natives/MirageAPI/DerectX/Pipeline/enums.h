#pragma once

namespace MirageAPI::DirectX::Pipeline
{
    public enum class DX12PrimitiveTopologyType
    {
        UNDEFINED = 0,
        POINT = 1,
        LINE = 2,
        TRIANGLE = 3,
        PATCH = 4
    };

    public enum class DX12BlendOptions
    {
        ZERO = 1,
        ONE = 2,
        SRC_COLOR = 3,
        INV_SRC_COLOR = 4,
        SRC_ALPHA = 5,
        INV_SRC_ALPHA = 6,
        DEST_ALPHA = 7,
        INV_DEST_ALPHA = 8,
        DEST_COLOR = 9,
        INV_DEST_COLOR = 10,
        SRC_ALPHA_SAT = 11,
        BLEND_FACTOR = 14,
        INV_BLEND_FACTOR = 15,
        SRC1_COLOR = 16,
        INV_SRC1_COLOR = 17,
        SRC1_ALPHA = 18,
        INV_SRC1_ALPHA = 19,
        ALPHA_FACTOR = 20,
        INV_ALPHA_FACTOR = 21,

        OP_ADD = 1,
        OP_SUBTRACT = 2,
        OP_REV_SUBTRACT = 3,
        OP_MIN = 4,
        OP_MAX = 5
    };

    public enum class DX12LogicOperators
    {
        CLEAR = 0,
        SET = CLEAR + 1,
        COPY = SET + 1,
        COPY_INVERTED = COPY + 1,
        NOOP = COPY_INVERTED + 1,
        INVERT = NOOP + 1,
        AND = INVERT + 1,
        NAND = AND + 1,
        OR = NAND + 1,
        NOR = OR + 1,
        XOR = NOR + 1,
        EQUIV = XOR + 1,
        AND_REVERSE = EQUIV + 1,
        AND_INVERTED = AND_REVERSE + 1,
        OR_REVERSE = AND_INVERTED + 1,
        OR_INVERTED = OR_REVERSE + 1
    };

    [System::Flags]
    public enum class DX12ColorWriteEnable : unsigned char
    {
        RED = 1,
        GREEN = 2,
        BLUE = 4,
        ALPHA = 8,
        ALL = RED | GREEN | BLUE | ALPHA
    };

    public enum class DX12FillMode
    {
        WIREFRAME = 2,
        SOLID = 3
    };

    public enum class DX12CullMode
    {
        NONE = 1,
        FRONT = 2,
        BACK = 3
    };

    public enum class DX12Filter
    {
        MIN_MAG_MIP_POINT = 0,
        MIN_MAG_POINT_MIP_LINEAR = 0x1,
        MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
        MIN_POINT_MAG_MIP_LINEAR = 0x5,
        MIN_LINEAR_MAG_MIP_POINT = 0x10,
        MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
        MIN_MAG_LINEAR_MIP_POINT = 0x14,
        MIN_MAG_MIP_LINEAR = 0x15,
        ANISOTROPIC = 0x55,
        COMPARISON_MIN_MAG_MIP_POINT = 0x80,
        COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
        COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
        COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
        COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
        COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
        COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
        COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
        COMPARISON_ANISOTROPIC = 0xd5,
        MINIMUM_MIN_MAG_MIP_POINT = 0x100,
        MINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
        MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
        MINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
        MINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
        MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
        MINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
        MINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
        MINIMUM_ANISOTROPIC = 0x155,
        MAXIMUM_MIN_MAG_MIP_POINT = 0x180,
        MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
        MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
        MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
        MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
        MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
        MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
        MAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
        MAXIMUM_ANISOTROPIC = 0x1d5
    };

    public enum class DX12TextureAddressMode
    {
        WRAP = 1,
        MIRROR = 2,
        CLAMP = 3,
        BORDER = 4,
        MIRROR_ONCE = 5
    };

    public enum class DX12ComparisonFunc
    {
        NEVER = 1,
        LESS = 2,
        EQUAL = 3,
        LESS_EQUAL = 4,
        GREATER = 5,
        NOT_EQUAL = 6,
        GREATER_EQUAL = 7,
        ALWAYS = 8
    };

    public enum class DX12BorderColor
    {
        TRANSPARENT_BLACK = 0,
        OPAQUE_BLACK = TRANSPARENT_BLACK + 1,
        OPAQUE_WHITE = OPAQUE_BLACK + 1
    };

    public enum class DX12RootSignatureVersion
    {
        V_1 = 0x1,
        V_1_0 = 0x1,
        V_1_1 = 0x2
    };
}
