#pragma once

#include <string>

#include "../DX12Object.h"
#include "DX12PipelineStateConfig.h"

namespace MirageAPI::DirectX::Pipeline
{
    public ref class DX12PipelineState : public DX12Object
    {
        ID3D12PipelineState* m_pso;
        ID3D12RootSignature* m_rootSignature;
        
        std::string* m_semanticNames;
        D3D12_INPUT_ELEMENT_DESC* m_inputLayoutsArr;
        UINT m_inputLayoutsLength;
        D3D12_ROOT_PARAMETER* m_rootParameters;
        UINT m_rootParametersLength;
        System::Collections::Generic::List<System::IntPtr>^ m_descriptorRanges;

        D3D12_ROOT_PARAMETER GenerateRootParameterDesc(DX12PipelineParameter param);

    public:
        DX12PipelineState(DX12PipelineStateConfig config);
        ~DX12PipelineState() { this->!DX12PipelineState(); }
        !DX12PipelineState();

        property ID3D12PipelineState* NativePSO { 
            ID3D12PipelineState* get() { return m_pso; } 
        }
    
        property ID3D12RootSignature* RootSignature { 
            ID3D12RootSignature* get() { return m_rootSignature; } 
        }
    };
}
