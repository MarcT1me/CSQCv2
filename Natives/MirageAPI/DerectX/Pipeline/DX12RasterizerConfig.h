#pragma once

namespace MirageAPI::DirectX
{
    public value struct DX12RasterizerConfig
    {
        DX12FillMode FillMode;
        DX12CullMode CullMode;
        
        bool FrontCounterClockwise;
        
        int DepthBias;
        float DepthBiasClamp;
        float SlopeScaledDepthBias;
        
        bool DepthClipEnable;
        bool MultisampleEnable;
        bool AntialiasedLineEnable;
        unsigned int ForcedSampleCount;
        bool ConservativeRasterEnabled;
        
        static property DX12RasterizerConfig Default
        {
            DX12RasterizerConfig get()
            {
                return {
                    DX12FillMode::Solid,
                    DX12CullMode::Back,
                    false,
                    D3D12_DEFAULT_DEPTH_BIAS,
                    D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
                    D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
                    true,
                    false,
                    false,
                    0,
                    false
                };
            }
        }
    };
}