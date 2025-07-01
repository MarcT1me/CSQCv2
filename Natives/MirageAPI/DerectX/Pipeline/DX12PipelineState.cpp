#include "pch.h"
#include "DX12PipelineState.h"

#include <iostream>
#include <ostream>
#include <msclr/marshal_cppstd.h>

#include "..\DX12Context.h"

namespace MirageAPI::DirectX
{
    DX12PipelineState::DX12PipelineState(
        DX12PipelineStateConfig config
    )
    {
        auto device = DX12Context::GetDevice();

        D3D12_ROOT_PARAMETER param = {};
        param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
        param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {
            1, &param,
            0, nullptr,
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
            throw gcnew System::Exception("Failed to create root signature");
        }
        m_rootSignature = rootSignature;

        // 2. Создаем PSO
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature = m_rootSignature;
        psoDesc.VS = config.VertexShader->GetNativeBytecode();
        psoDesc.PS = config.PixelShader->GetNativeBytecode();

        // Ручная настройка растеризатора
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

        // Ручная настройка блендинга
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
        psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = static_cast<UINT8>(config.BlendState.
            RenderTargetWriteMask);

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
        for (int i = 0; i < m_inputLayoutsLength; i++)
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
        
        ID3D12PipelineState* pso;
        hr = device->CreateGraphicsPipelineState(
            &psoDesc,
            IID_PPV_ARGS(&pso)
        );
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Failed to create pipeline state");
        }
        m_pso = pso;
    }

    DX12PipelineState::~DX12PipelineState() { this->!DX12PipelineState(); }

    DX12PipelineState::!DX12PipelineState()
    {
        if (m_pso)
        {
            m_pso->Release();
            m_pso = nullptr;
        }
        if (m_rootSignature)
        {
            m_rootSignature->Release();
            m_rootSignature = nullptr;
        }
        if (m_inputLayoutsArr)
        {
            delete[] m_inputLayoutsArr;
            m_inputLayoutsArr = nullptr;
        }
        if (m_semanticNames)
        {
            delete[] m_semanticNames;
            m_semanticNames = nullptr;
        }
    }
}
