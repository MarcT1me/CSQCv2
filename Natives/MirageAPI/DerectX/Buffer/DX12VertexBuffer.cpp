#include "pch.h"
#include "DX12VertexBuffer.h"

namespace MirageAPI::DirectX
{
    DX12VertexBuffer::DX12VertexBuffer(
        unsigned int elementCount,
        unsigned int stride
    ) : DX12Buffer(DX12ResourceConfig::VertexBufferConfig(elementCount, stride))
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
