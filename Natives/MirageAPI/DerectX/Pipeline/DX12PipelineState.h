#pragma once

#include <string>

#include "DX12PipelineStateConfig.h"
#include "../Shader/DX12Shader.h"


namespace MirageAPI::DirectX::Pipeline
{
    public ref class DX12PipelineState : public DX12Object
    {
        ID3D12RootSignature* m_rootSignature;
        ID3D12PipelineState* m_pso;

        CSList<System::IntPtr>^ m_descriptorRanges;
        D3D12_ROOT_PARAMETER* m_rootParameters;
        UINT m_rootParametersLength;

        D3D12_STATIC_SAMPLER_DESC* m_samplers;
        UINT m_samplersLength;

        std::string* m_semanticNames;
        D3D12_INPUT_ELEMENT_DESC* m_inputLayoutsArr;
        UINT m_inputLayoutsLength;

    public:
        DX12PipelineState(
            Shader::DX12Shader^ vertexShader,
            Shader::DX12Shader^ pixelShader,
            DX12PipelineStateConfig^ config
        );

        ~DX12PipelineState() { this->!DX12PipelineState(); }
        !DX12PipelineState();

        property ID3D12PipelineState* NativePSO
        {
            ID3D12PipelineState* get() { return m_pso; }
        }
        property ID3D12RootSignature* RootSignature
        {
            ID3D12RootSignature* get() { return m_rootSignature; }
        }
    };
}
