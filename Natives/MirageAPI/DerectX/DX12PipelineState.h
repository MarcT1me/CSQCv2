#pragma once

#include "DX12ContextConfig.h"
#include "DX12Shader.h"

namespace MirageAPI::DirectX
{
    public ref class DX12PipelineState
    {
        ID3D12PipelineState* m_pso;
        ID3D12RootSignature* m_rootSignature;

    public:
        DX12PipelineState(
            DX12Shader^ vertexShader,
            DX12Shader^ pixelShader,
            DX12BufferFormat rtvFormat
        );
        ~DX12PipelineState();
        !DX12PipelineState();

        property ID3D12PipelineState* NativePSO { 
            ID3D12PipelineState* get() { return m_pso; } 
        }
    
        property ID3D12RootSignature* RootSignature { 
            ID3D12RootSignature* get() { return m_rootSignature; } 
        }
    };
}