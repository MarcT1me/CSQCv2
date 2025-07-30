#include "pch.h"
#include "DX12PipelineState.h"

#include <iostream>
#include <ostream>

namespace MirageAPI::DirectX::Pipeline
{
    inline D3D12_ROOT_PARAMETER DX12PipelineState::GenerateRootParameterDesc(DX12PipelineParameter param)
    {
        D3D12_ROOT_PARAMETER d3dParam;
        d3dParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

        switch (param.Type)
        {
        case DX12ResourceType::Constants:
            {
                d3dParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;

                d3dParam.Constants.Num32BitValues = param.NumConstants;
                d3dParam.Constants.ShaderRegister = param.RegisterSlot;
                d3dParam.Constants.RegisterSpace = param.RegisterSpace;
                break;
            }
        case DX12ResourceType::ConstantBuffer:
            {
                d3dParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;

                d3dParam.Descriptor.ShaderRegister = param.RegisterSlot;
                d3dParam.Descriptor.RegisterSpace = param.RegisterSpace;
                break;
            }
        case DX12ResourceType::StructuredBuffer:
            {
                d3dParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;

                d3dParam.Descriptor.ShaderRegister = param.RegisterSlot;
                d3dParam.Descriptor.RegisterSpace = param.RegisterSpace;
                break;
            }
        case DX12ResourceType::Texture:
            {
                d3dParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                d3dParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

                // Создаем диапазон в куче
                D3D12_DESCRIPTOR_RANGE* range = new D3D12_DESCRIPTOR_RANGE();
                range->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                range->NumDescriptors = 1;
                range->BaseShaderRegister = param.RegisterSlot;
                range->RegisterSpace = param.RegisterSpace;
                range->OffsetInDescriptorsFromTableStart = 0;

                d3dParam.DescriptorTable.NumDescriptorRanges = 1;
                d3dParam.DescriptorTable.pDescriptorRanges = range;

                // Сохраняем указатель для последующего удаления
                m_descriptorRanges->Add(System::IntPtr(range));
                break;
            }
        default:
            {
                throw gcnew System::NotSupportedException("Unsupported resource type");
            }
        }

        // ReSharper disable once CppSomeObjectMembersMightNotBeInitialized
        return d3dParam;
    }

    DX12PipelineState::DX12PipelineState(
        DX12PipelineStateConfig config
    ) : m_descriptorRanges(gcnew System::Collections::Generic::List<System::IntPtr>())
    {
        // generating root params
        if (config.RootParams)
        {
            m_rootParametersLength = config.RootParams->Length;
            m_rootParameters = new D3D12_ROOT_PARAMETER[m_rootParametersLength];

            for (UINT i = 0; i < m_rootParametersLength; i++)
                m_rootParameters[i] = GenerateRootParameterDesc(config.RootParams[i]);
        }

        // sampler settings
        D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 16;
        samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        samplerDesc.MinLOD = 0.0f;
        samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
        samplerDesc.ShaderRegister = 0;
        samplerDesc.RegisterSpace = 0;
        samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        // creating root signature
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {
            m_rootParametersLength, m_rootParameters,
            1, &samplerDesc,
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
        };

        ID3DBlob* signatureBlob;
        ID3DBlob* errorBlob;
        HRESULT hr = D3D12SerializeRootSignature(
            &rootSignatureDesc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            &signatureBlob,
            &errorBlob
        );

        if (FAILED(hr))
        {
            if (errorBlob)
            {
                LPCSTR errMsg = static_cast<LPCSTR>(errorBlob->GetBufferPointer());
                System::String^ message = gcnew System::String(errMsg);
                errorBlob->Release();
                throw gcnew System::Exception("Root signature error: " + message);
            }
            throw gcnew System::Exception("Failed to serialize root signature");
        }

        ID3D12RootSignature* rootSignature;
        hr = device->CreateRootSignature(
            0,
            signatureBlob->GetBufferPointer(),
            signatureBlob->GetBufferSize(),
            IID_PPV_ARGS(&rootSignature)
        );
        signatureBlob->Release();
        if (errorBlob) errorBlob->Release();

        if (FAILED(hr))
        {
            if (hr == DXGI_ERROR_DEVICE_REMOVED)
            {
                HRESULT deviceRemovedReason = device->GetDeviceRemovedReason();
                System::String^ reasonMsg = "Device removed reason: " + deviceRemovedReason;
                throw gcnew System::Exception(reasonMsg);
            }
            throw gcnew System::Exception("Failed to create root signature: " + hr);
        }
        m_rootSignature = rootSignature;

        // creating pso
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature = m_rootSignature;
        psoDesc.VS = config.VertexShader->NativeBytecode;
        psoDesc.PS = config.PixelShader->NativeBytecode;

        // rasterizer settings
        psoDesc.RasterizerState.FillMode = static_cast<D3D12_FILL_MODE>(config.RasterizerState.FillMode);
        psoDesc.RasterizerState.CullMode = static_cast<D3D12_CULL_MODE>(config.RasterizerState.CullMode);

        psoDesc.RasterizerState.FrontCounterClockwise = config.RasterizerState.FrontCounterClockwise;

        psoDesc.RasterizerState.DepthBias = config.RasterizerState.DepthBias;
        psoDesc.RasterizerState.DepthBiasClamp = config.RasterizerState.DepthBiasClamp;
        psoDesc.RasterizerState.SlopeScaledDepthBias = config.RasterizerState.SlopeScaledDepthBias;

        psoDesc.RasterizerState.DepthClipEnable = config.RasterizerState.DepthClipEnable;
        psoDesc.RasterizerState.MultisampleEnable = config.RasterizerState.MultisampleEnable;
        psoDesc.RasterizerState.AntialiasedLineEnable = config.RasterizerState.AntialiasedLineEnable;
        psoDesc.RasterizerState.ForcedSampleCount = config.RasterizerState.ForcedSampleCount;
        psoDesc.RasterizerState.ConservativeRaster = config.RasterizerState.ConservativeRasterEnabled
                                                         ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON
                                                         : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        // blend settings
        psoDesc.BlendState.AlphaToCoverageEnable = FALSE;
        psoDesc.BlendState.IndependentBlendEnable = FALSE;

        psoDesc.BlendState.RenderTarget[0].LogicOpEnable = config.BlendState.LogicOpEnable;
        psoDesc.BlendState.RenderTarget[0].LogicOp = static_cast<D3D12_LOGIC_OP>(config.BlendState.LogicOp);

        psoDesc.BlendState.RenderTarget[0].BlendEnable = config.BlendState.BlendEnable;
        psoDesc.BlendState.RenderTarget[0].SrcBlend = static_cast<D3D12_BLEND>(config.BlendState.SrcBlend);
        psoDesc.BlendState.RenderTarget[0].DestBlend = static_cast<D3D12_BLEND>(config.BlendState.DestBlend);
        psoDesc.BlendState.RenderTarget[0].BlendOp = static_cast<D3D12_BLEND_OP>(config.BlendState.BlendOp);
        psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = static_cast<D3D12_BLEND>(config.BlendState.SrcBlendAlpha);
        psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = static_cast<D3D12_BLEND>(config.BlendState.DestBlendAlpha);
        psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = static_cast<D3D12_BLEND_OP>(config.BlendState.BlendOpAlpha);
        psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = static_cast<UINT8>(
            config.BlendState.RenderTargetWriteMask);

        // Настройки глубины
        psoDesc.DepthStencilState.DepthEnable = config.DepthEnable;
        psoDesc.DepthStencilState.StencilEnable = config.StencilEnable;
        psoDesc.DSVFormat = static_cast<DXGI_FORMAT>(config.DSVFormat);

        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(config.PrimitiveTopologyType);
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = static_cast<DXGI_FORMAT>(config.RTVFormat);
        psoDesc.SampleDesc.Count = 1;

        m_inputLayoutsLength = config.InputLayouts->Length;
        m_inputLayoutsArr = new D3D12_INPUT_ELEMENT_DESC[m_inputLayoutsLength];
        m_semanticNames = new std::string[m_inputLayoutsLength];
        for (UINT i = 0; i < m_inputLayoutsLength; i++)
        {
            DX12InputElement inputLayout = config.InputLayouts[i];
            m_semanticNames[i] = msclr::interop::marshal_as<std::string>(inputLayout.SemanticName);
            m_inputLayoutsArr[i] =
            {
                m_semanticNames[i].c_str(),
                inputLayout.SemanticIndex,
                static_cast<DXGI_FORMAT>(inputLayout.Format),
                inputLayout.InputSlot,
                inputLayout.Offset,
                D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
                inputLayout.DataStepRate
            };
        }
        psoDesc.InputLayout = {m_inputLayoutsArr, m_inputLayoutsLength};

        // pso himself
        ID3D12PipelineState* pso;
        CheckHResult(
            device->CreateGraphicsPipelineState(
                &psoDesc,
                IID_PPV_ARGS(&pso)
            ),
            "Failed to create pipeline state"
        );
        if (!pso) throw gcnew DXException("Failed to create pipeline");
        m_pso = pso;
    }

    DX12PipelineState::!DX12PipelineState()
    {
        Validate();

        DXSimpleRelease(m_pso);
        DXSimpleRelease(m_rootSignature);

        DXSimpleDeleteArr(m_inputLayoutsArr);
        DXSimpleDeleteArr(m_semanticNames);

        if (m_descriptorRanges)
        {
            for each (auto desc in m_descriptorRanges)
            {
                delete desc;
            }
            m_descriptorRanges->Clear();
        }
        if (m_rootParameters)
        {
            for (unsigned int i = 0; i < m_rootParametersLength; i++)
            {
                if (m_rootParameters[i].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
                {
                    delete m_rootParameters[i].DescriptorTable.pDescriptorRanges;
                }
            }
            DXSimpleDeleteArr(m_rootParameters);
        }
    }
}
