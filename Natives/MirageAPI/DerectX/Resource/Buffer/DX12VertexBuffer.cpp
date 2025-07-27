#include "pch.h"
#include "DX12VertexBuffer.h"

namespace MirageAPI::DirectX::Resource
{
    void DX12VertexBuffer::Bind(ID3D12GraphicsCommandList* commandList)
    {
        Validate();
        
        D3D12_VERTEX_BUFFER_VIEW view;
        view.BufferLocation = GPUAddress;
        view.SizeInBytes = m_size;
        view.StrideInBytes = m_stride;

        commandList->IASetVertexBuffers(0, 1, &view);
    }
}
