#pragma once

namespace MirageAPI::DirectX::Shader
{
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
}
