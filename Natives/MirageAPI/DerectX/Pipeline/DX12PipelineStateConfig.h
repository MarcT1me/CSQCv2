#pragma once

#include "..\Shader\DX12Shader.h"
#include "DX12RasterizerConfig.h"
#include "DX12BlendConfig.h"
#include "DX12PipelineParameter.h"

namespace MirageAPI::DirectX::Pipeline
{
    public value struct DX12InputElement
    {
        System::String^ SemanticName;
        uint32_t SemanticIndex;
        DX12ResourceFormat Format;
        UINT InputSlot;
        UINT Offset;
        UINT DataStepRate;
    };

    public value struct DX12PipelineStateConfig
    {
        Shader::DX12Shader^ VertexShader;
        Shader::DX12Shader^ PixelShader;
        
        array<DX12InputElement>^ InputLayouts;
        array<DX12PipelineParameter>^ RootParams;
        
        DX12RasterizerConfig RasterizerState;
        DX12BlendConfig BlendState;
        bool DepthEnable;
        bool StencilEnable;

        DX12ResourceFormat RTVFormat;
        DX12ResourceFormat DSVFormat;

        DX12PrimitiveTopologyType PrimitiveTopologyType;

        static property DX12PipelineStateConfig Default
        {
            DX12PipelineStateConfig get()
            {
                DX12PipelineStateConfig stateConfig = {};
                stateConfig.RasterizerState = DX12RasterizerConfig::Default;
                stateConfig.BlendState = DX12BlendConfig::Default;
                stateConfig.DepthEnable = false;
                stateConfig.StencilEnable = false;
                stateConfig.DSVFormat = DX12ResourceFormat::Unknown;
                stateConfig.PrimitiveTopologyType = DX12PrimitiveTopologyType::Triangle;
                stateConfig.RTVFormat = DX12ResourceFormat::RGBA8_UNORM;
                return stateConfig;
            }
        }
    };
}
