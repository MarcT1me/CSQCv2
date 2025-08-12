#include "pch.h"
#include "DX12PipelineState.h"

#include <iostream>
#include <ostream>

namespace MirageAPI::DirectX::Pipeline
{
    DX12PipelineState::DX12PipelineState(
        Shader::DX12Shader^ vertexShader,
        Shader::DX12Shader^ pixelShader,
        DX12PipelineStateConfig^ config
    ) : m_descriptorRanges(gcnew CSList<IntPtr>())
    {
        // generating root params
        if (config->RootParams)
        {
            m_rootParametersLength = config->RootParams->Length;
            m_rootParameters = new D3D12_ROOT_PARAMETER[m_rootParametersLength];

            for (UINT i = 0; i < m_rootParametersLength; i++)
                m_rootParameters[i] = config->RootParams[i]->GenerateNativeDesc(m_descriptorRanges);
        }
        // samplers
        if (config->SamplerConfigs)
        {
            m_samplersLength = config->SamplerConfigs->Length;
            m_samplers = new D3D12_STATIC_SAMPLER_DESC[m_samplersLength];

            for (UINT i = 0; i < m_samplersLength; i++)
                m_samplers[i] = config->SamplerConfigs[i]->GenerateNativeStaticDesc();
        }
        // creating root signature desc
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {
            m_rootParametersLength, m_rootParameters,
            m_samplersLength, m_samplers,
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
        };

        ID3DBlob* signatureBlob;
        ID3DBlob* errorBlob;
        HRESULT hr = D3D12SerializeRootSignature(
            &rootSignatureDesc,
            static_cast<D3D_ROOT_SIGNATURE_VERSION>(config->RootSignatureVersion),
            &signatureBlob,
            &errorBlob
        );
        if (FAILED(hr))
        {
            if (errorBlob)
            {
                LPCSTR errMsg = static_cast<LPCSTR>(errorBlob->GetBufferPointer());
                String^ message = gcnew String(errMsg);
                errorBlob->Release();
                throw gcnew DXException("Root signature error: " + message);
            }
            throw gcnew DXException("Failed to serialize root signature");
        }

        ID3D12RootSignature* rootSignature;
        hr = device->CreateRootSignature(
            0,
            signatureBlob->GetBufferPointer(),
            signatureBlob->GetBufferSize(),
            IID_PPV_ARGS(&rootSignature)
        );
        SimpleRelease(signatureBlob);
        SimpleRelease(errorBlob);
        CheckHResult(hr, "Failed to create root signature");
        m_rootSignature = rootSignature;

        // pso
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature = m_rootSignature;
        // shaders
        psoDesc.VS = vertexShader->NativeBytecode;
        psoDesc.PS = pixelShader->NativeBytecode;
        // rasterizer
        psoDesc.RasterizerState = config->RasterizerState->GenerateNativeDesc();
        // rtv
        config->BlendState->SetupInPSODesc(&psoDesc);
        // dss
        psoDesc.DSVFormat = static_cast<DXGI_FORMAT>(config->DSVFormat);
        psoDesc.DepthStencilState.DepthEnable = config->DepthStencilEnable;
        psoDesc.DepthStencilState.StencilEnable = config->StencilEnable;
        psoDesc.DepthStencilState.DepthFunc = static_cast<D3D12_COMPARISON_FUNC>(config->DepthFunc);
        psoDesc.DepthStencilState.DepthWriteMask = config->DepthWriteEnable
                                                       ? D3D12_DEPTH_WRITE_MASK_ALL
                                                       : D3D12_DEPTH_WRITE_MASK_ZERO;
        // primitive
        psoDesc.PrimitiveTopologyType = static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(config->PrimitiveTopologyType);
        // sample
        psoDesc.SampleMask = config->SampleMask;
        psoDesc.SampleDesc = {m_samplersLength ? m_samplersLength : 1, 0};
        // inputs
        m_inputLayoutsLength = config->InputLayouts->Length;
        m_inputLayoutsArr = new D3D12_INPUT_ELEMENT_DESC[m_inputLayoutsLength];
        m_semanticNames = new std::string[m_inputLayoutsLength];
        for (UINT i = 0; i < m_inputLayoutsLength; i++)
        {
            DX12InputElement^ inputLayout = config->InputLayouts[i];
            String^ str = inputLayout->Name;
            m_semanticNames[i] = msclr::interop::marshal_as<std::string>(str);
            m_inputLayoutsArr[i] =
            {
                m_semanticNames[i].c_str(),
                inputLayout->Index,
                static_cast<DXGI_FORMAT>(inputLayout->Format),
                inputLayout->Slot,
                inputLayout->Offset,
                (inputLayout->IsInstanceData
                     ? D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA
                     : D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA),
                inputLayout->DataStepRate
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
        ThrowIfNull(pso, "PSO creation failed");
        m_pso = pso;
    }

    DX12PipelineState::!DX12PipelineState()
    {
        Validate();

        SimpleRelease(m_pso);
        SimpleRelease(m_rootSignature);

        SimpleDeleteArr(m_rootParameters);
        SimpleDeleteArr(m_samplers);
        SimpleDeleteArr(m_inputLayoutsArr);
        SimpleDeleteArr(m_semanticNames);
        SimpleDeleteArr(m_descriptorRanges);
        SimpleDelete(m_descriptorRanges);
    }
}
