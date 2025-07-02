#include "pch.h"
#include "DX12IndexBuffer.h"

namespace MirageAPI::DirectX
{
    DX12IndexBuffer::DX12IndexBuffer(
        unsigned int elementCount,
        DX12ResourceFormat format
    ) : DX12Buffer(DX12ResourceConfig::IndexBufferConfig(elementCount, format))
    {
    }

    void DX12IndexBuffer::Bind(ID3D12GraphicsCommandList* commandList)
    {
        D3D12_INDEX_BUFFER_VIEW view;
        view.BufferLocation = GPUAddress;
        view.SizeInBytes = m_size;
        view.Format = static_cast<DXGI_FORMAT>(m_format);

        commandList->IASetIndexBuffer(&view);
    }
}
