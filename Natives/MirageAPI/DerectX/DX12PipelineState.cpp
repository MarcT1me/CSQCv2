#include "pch.h"
#include "DX12PipelineState.h"
#include "DX12Context.h"

namespace MirageAPI::DirectX
{
    DX12PipelineState::DX12PipelineState(
        DX12Shader^ vertexShader,
        DX12Shader^ pixelShader,
        DX12BufferFormat rtvFormat
    )
    {
        auto device = DX12Context::GetDevice();

        // 1. Создаем корневую сигнатуру (упрощенная версия)
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

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
        psoDesc.VS = vertexShader->GetNativeBytecode();
        psoDesc.PS = pixelShader->GetNativeBytecode();

        // Ручная настройка растеризатора
        psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        psoDesc.RasterizerState.FrontCounterClockwise = FALSE;
        psoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        psoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        psoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        psoDesc.RasterizerState.DepthClipEnable = TRUE;
        psoDesc.RasterizerState.MultisampleEnable = FALSE;
        psoDesc.RasterizerState.AntialiasedLineEnable = FALSE;
        psoDesc.RasterizerState.ForcedSampleCount = 0;
        psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        // Ручная настройка блендинга
        psoDesc.BlendState.AlphaToCoverageEnable = FALSE;
        psoDesc.BlendState.IndependentBlendEnable = FALSE;
        psoDesc.BlendState.RenderTarget[0].BlendEnable = FALSE;
        psoDesc.BlendState.RenderTarget[0].LogicOpEnable = FALSE;
        psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
        psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
        psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
        psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
        psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
        psoDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        psoDesc.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
        psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        // Настройки глубины
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;

        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = static_cast<DXGI_FORMAT>(rtvFormat);
        psoDesc.SampleDesc.Count = 1;

        D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
        };

        psoDesc.InputLayout = {inputLayout, _countof(inputLayout)};

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
        if (m_pso) m_pso->Release();
        if (m_rootSignature) m_rootSignature->Release();
    }
}
