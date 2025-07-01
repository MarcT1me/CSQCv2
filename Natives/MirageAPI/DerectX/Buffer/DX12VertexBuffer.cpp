#include "pch.h"
#include "DX12VertexBuffer.h"

namespace MirageAPI::DirectX
{
    DX12VertexBuffer::DX12VertexBuffer(DX12ResourceConfig config)
        : DX12Buffer(config)
    {
    }

    void DX12VertexBuffer::Bind(ID3D12GraphicsCommandList* commandList)
    {
        D3D12_VERTEX_BUFFER_VIEW view;
        view.BufferLocation = GPUAddress;
        view.SizeInBytes = m_size;
        view.StrideInBytes = m_stride;

        commandList->IASetVertexBuffers(0, 1, &view);
    }
}
