#pragma once

#include "enums.h"

namespace MirageAPI::DirectX::Pipeline
{
    public ref struct DX12RootParameter
    {
        DX12ResourceType Type;
        UINT Slot = 0;
        UINT Size = 0;
        UINT Count = 0;
        Shader::DX12ShaderVisibility ShaderVisibility = Shader::DX12ShaderVisibility::All;

        DX12RootParameter(
            DX12ResourceType type,
            Shader::DX12ShaderVisibility shaderVisibility
        ) : Type(type),
            ShaderVisibility(shaderVisibility)
        {
        }

    internal:
        D3D12_ROOT_PARAMETER GenerateNativeDesc(CSList<IntPtr>^ rangesList)
        {
            D3D12_ROOT_PARAMETER param;
            param.ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(Type);
            param.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(ShaderVisibility);

            if (Type == DX12ResourceType::Constants ||
                Type == DX12ResourceType::ConstantBuffer ||
                Type == DX12ResourceType::StructuredBuffer)
            {
                param.Constants.ShaderRegister = Slot;
                param.Constants.RegisterSpace = Size;

                if (Type == DX12ResourceType::Constants)
                    param.Constants.Num32BitValues = Count;
            }
            else if (Type == DX12ResourceType::Texture)
            {
                // Создаем диапазон в куче
                D3D12_DESCRIPTOR_RANGE* range = new D3D12_DESCRIPTOR_RANGE();
                range->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                range->NumDescriptors = 1;
                range->BaseShaderRegister = Slot;
                range->RegisterSpace = Size;
                range->OffsetInDescriptorsFromTableStart = 0;

                param.DescriptorTable.NumDescriptorRanges = 1;
                param.DescriptorTable.pDescriptorRanges = range;

                rangesList->Add(IntPtr(range));
            }
            else
            {
                throw gcnew DXException("Unsupported resource type");
            }
            // ReSharper disable once CppSomeObjectMembersMightNotBeInitialized
            return param;
        }
    };

    public ref struct DX12SamplerConfig
    {
        UINT Slot;
        Shader::DX12ShaderVisibility ShaderVisibility;

        DX12Filter Filter = DX12Filter::MIN_MAG_MIP_LINEAR;
        DX12TextureAddressMode AddressU = DX12TextureAddressMode::Wrap;
        DX12TextureAddressMode AddressV = DX12TextureAddressMode::Wrap;
        DX12TextureAddressMode AddressW = DX12TextureAddressMode::Wrap;
        float MipLODBias = 0;
        UINT MaxAnisotropy = 16;
        DX12ComparisonFunc ComparisonFunc = DX12ComparisonFunc::Always;
        DX12BorderColor BorderColor = DX12BorderColor::TransparentBlack;
        float MinLOD = 0;
        float MaxLOD = D3D12_FLOAT32_MAX;
        UINT Size = 0u;

        DX12SamplerConfig(
            UINT slot,
            Shader::DX12ShaderVisibility shaderVisibility
        ) : Slot(slot),
            ShaderVisibility(shaderVisibility)
        {
        }

    internal:
        D3D12_STATIC_SAMPLER_DESC GenerateNativeStaticDesc()
        {
            D3D12_STATIC_SAMPLER_DESC desc;

            desc.Filter = static_cast<D3D12_FILTER>(Filter);
            desc.AddressU = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(AddressU);
            desc.AddressV = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(AddressV);
            desc.AddressW = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(AddressW);
            desc.MipLODBias = MipLODBias;
            desc.MaxAnisotropy = MaxAnisotropy;
            desc.ComparisonFunc = static_cast<D3D12_COMPARISON_FUNC>(ComparisonFunc);
            desc.BorderColor = static_cast<D3D12_STATIC_BORDER_COLOR>(BorderColor);
            desc.MinLOD = MinLOD;
            desc.MaxLOD = MaxLOD;
            desc.ShaderRegister = Slot;
            desc.RegisterSpace = Size;
            desc.ShaderVisibility = static_cast<D3D12_SHADER_VISIBILITY>(ShaderVisibility);

            return desc;
        }
    };

    public ref struct DX12RasterizerConfig
    {
        DX12FillMode FillMode = DX12FillMode::Solid;
        DX12CullMode CullMode = DX12CullMode::Back;

        bool FrontCounterClockwise = false;

        int DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        float DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        float SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        bool DepthClipEnable = true;

        bool MultisampleEnable = false;
        bool AntialiasedLineEnable = false;
        UINT ForcedSampleCount = 0;
        bool ConservativeRasterEnabled = false;

    internal:
        D3D12_RASTERIZER_DESC GenerateNativeDesc()
        {
            D3D12_RASTERIZER_DESC desc;

            desc.FillMode = static_cast<D3D12_FILL_MODE>(FillMode);
            desc.CullMode = static_cast<D3D12_CULL_MODE>(CullMode);

            desc.FrontCounterClockwise = FrontCounterClockwise;

            desc.DepthBias = DepthBias;
            desc.DepthBiasClamp = DepthBiasClamp;
            desc.SlopeScaledDepthBias = SlopeScaledDepthBias;
            desc.DepthClipEnable = DepthClipEnable;

            desc.MultisampleEnable = MultisampleEnable;
            desc.AntialiasedLineEnable = AntialiasedLineEnable;
            desc.ForcedSampleCount = ForcedSampleCount;
            desc.ConservativeRaster = ConservativeRasterEnabled
                                          ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON
                                          : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
            return desc;
        }
    };

    public ref struct DX12BlendConfig
    {
        bool AlphaToCoverageEnable = false;
        bool IndependentBlendEnable = false;

        UINT RenderTargetCount = 1;

        DX12ResourceFormat RTVFormat = DX12ResourceFormat::RGBA8_UNORM;

        bool BlendEnable;
        DX12BlendOptions SrcBlend = DX12BlendOptions::SRC_ALPHA;
        DX12BlendOptions DestBlend = DX12BlendOptions::INV_SRC_ALPHA;
        DX12BlendOptions BlendOp = DX12BlendOptions::OP_ADD;
        DX12BlendOptions SrcBlendAlpha = DX12BlendOptions::ONE;
        DX12BlendOptions DestBlendAlpha = DX12BlendOptions::ZERO;
        DX12BlendOptions BlendOpAlpha = DX12BlendOptions::OP_ADD;

        bool LogicOpEnable;
        DX12LogicOperators LogicOp = DX12LogicOperators::NOOP;

        DX12ColorWriteEnable RenderTargetWriteMask = DX12ColorWriteEnable::All;

        DX12BlendConfig(
            bool blendEnable,
            bool logicOpEnable
        ) :
            BlendEnable(blendEnable),
            LogicOpEnable(logicOpEnable)
        {
        }

    internal:
        void SetupInPSODesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC* psoDesc)
        {
            psoDesc->NumRenderTargets = RenderTargetCount;
            D3D12_BLEND_DESC desc;
            desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
            desc.IndependentBlendEnable = IndependentBlendEnable;

            for (UINT i = 0; i < RenderTargetCount; i++)
            {
                psoDesc->RTVFormats[0] = static_cast<DXGI_FORMAT>(RTVFormat);
                SetupSingleRTV(&desc, i);
            }
            psoDesc->BlendState = desc;
        }

        void SetupSingleRTV(D3D12_BLEND_DESC* desc, UINT renderTargetIndex)
        {
            desc->RenderTarget[renderTargetIndex].LogicOpEnable = LogicOpEnable;
            desc->RenderTarget[renderTargetIndex].LogicOp = static_cast<D3D12_LOGIC_OP>(LogicOp);
            desc->RenderTarget[renderTargetIndex].BlendEnable = BlendEnable;
            desc->RenderTarget[renderTargetIndex].SrcBlend = static_cast<D3D12_BLEND>(SrcBlend);
            desc->RenderTarget[renderTargetIndex].DestBlend = static_cast<D3D12_BLEND>(DestBlend);
            desc->RenderTarget[renderTargetIndex].BlendOp = static_cast<D3D12_BLEND_OP>(BlendOp);
            desc->RenderTarget[renderTargetIndex].SrcBlendAlpha = static_cast<D3D12_BLEND>(SrcBlendAlpha);
            desc->RenderTarget[renderTargetIndex].DestBlendAlpha = static_cast<D3D12_BLEND>(DestBlendAlpha);
            desc->RenderTarget[renderTargetIndex].BlendOpAlpha = static_cast<D3D12_BLEND_OP>(BlendOpAlpha);
            desc->RenderTarget[renderTargetIndex].RenderTargetWriteMask = static_cast<UINT8>(RenderTargetWriteMask);
        }
    };

    public ref struct DX12InputElement
    {
        String^ Name;
        DX12ResourceFormat Format;
        UINT Offset;
        uint32_t Index = 0;
        UINT Slot = 0;
        UINT DataStepRate = 0;
        bool IsInstanceData = false;

        DX12InputElement(
            String^ name,
            DX12ResourceFormat format,
            UINT offset
        ) : Name(name),
            Format(format),
            Offset(offset)
        {
        }
    };

    public ref struct DX12PipelineStateConfig
    {
        // root signature
        array<DX12RootParameter^>^ RootParams;
        array<DX12SamplerConfig^>^ SamplerConfigs;
        DX12RootSignatureVersion RootSignatureVersion = DX12RootSignatureVersion::V_1_0;
        // pso
        DX12RasterizerConfig^ RasterizerState;
        DX12BlendConfig^ BlendState;
        array<DX12InputElement^>^ InputLayouts;
        // dss
        bool DepthWriteEnable = false;
        DX12ComparisonFunc DepthFunc = DX12ComparisonFunc::Less;
        bool DepthStencilEnable = false;
        bool StencilEnable = false;
        // other
        UINT SampleMask = UINT_MAX;
        DX12ResourceFormat DSVFormat = DX12ResourceFormat::Unknown;
        DX12PrimitiveTopologyType PrimitiveTopologyType = DX12PrimitiveTopologyType::Triangle;

        DX12PipelineStateConfig(
            // root signature
            array<DX12RootParameter^>^ rootParams,
            array<DX12SamplerConfig^>^ samplerConfigs,
            // pso
            DX12RasterizerConfig^ rasterizerConfig,
            DX12BlendConfig^ blendConfig,
            array<DX12InputElement^>^ inputLayouts
        ) : RootParams(rootParams),
            SamplerConfigs(samplerConfigs),
            RasterizerState(rasterizerConfig),
            BlendState(blendConfig),
            InputLayouts(inputLayouts)
        {
        }
    };
}
