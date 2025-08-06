#pragma once

namespace MirageAPI::DirectX::Shader
{
    public enum class DX12ShaderType
    {
        Vertex = 0,
        Pixel = 1,
        Geometry = 2,
        Compute = 3,
        Domain = 4,
        Hull = 5
    };

    [System::Flags]
    public enum class DX12ShaderCompileFlags
    {
        None = 0,
        Debug = 1 << 0,
        SkipValidation = 1 << 1,
        SkipOptimization = 1 << 2,
        PackMatrixRowMajor = 1 << 3,
        PackMatrixColumnMajor = 1 << 4,
        PartialPrecision = 1 << 5,
        ForceVSSoftwareNoOpt = 1 << 6,
        ForcePSSoftwareNoOpt = 1 << 7,
        NoPreShader = 1 << 8,
        AvoidFlowControl = 1 << 9,
        PreferFlowControl = 1 << 10,
        EnableStrictness = 1 << 11,
        EnableBackwardsCompatibility = 1 << 12,
        IEEE_Strictness = 1 << 13,
        OptimizationLevel0 = 1 << 14,
        OptimizationLevel1 = 0,
        OptimizationLevel2 = 1 << 14 | 1 << 15,
        OptimizationLevel3 = 1 << 15,
        Reserved16 = 1 << 16,
        Reserved17 = 1 << 17,
        WarningsAreErrors = 1 << 18,
        ResourcesMayAlias = 1 << 19,
        EnableUnboundedDescriptorTables = 1 << 20,
        AllResourcesBound = 1 << 21,
        DebugNameForSource = 1 << 22,
        DebugNameForBinary = 1 << 23
    };

    public enum class DX12ShaderVisibility
    {
        All = 0,
        Vertex = 1,
        Hull = 2,
        Domain = 3,
        Geometry = 4,
        Pixel = 5,
        Amplification = 6,
        Mesh = 7
    };
}
