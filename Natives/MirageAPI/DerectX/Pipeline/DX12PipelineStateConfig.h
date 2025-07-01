#pragma once

#include <string>

#include "..\Shader\DX12Shader.h"
#include "DX12RasterizerConfig.h"
#include "DX12BlendConfig.h"

namespace MirageAPI::DirectX
{
    public value struct DX12InputElement
    {
        System::String^ SemanticName;
        uint32_t SemanticIndex;
        DX12ResourceFormat Format;
        unsigned int InputSlot;
        unsigned int Offset;
        unsigned int DataStepRate;
    };

    public value struct DX12PipelineStateConfig
    {
        DX12Shader^ VertexShader;
        DX12Shader^ PixelShader;
        array<DX12InputElement>^ InputLayouts;

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
