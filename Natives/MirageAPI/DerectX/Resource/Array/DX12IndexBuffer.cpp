#include "pch.h"
#include "DX12IndexBuffer.h"

namespace MirageAPI::DirectX::Resource
{
    void DX12IndexBuffer::Bind(ID3D12GraphicsCommandList* commandList)
    {
        Validate();
        
        D3D12_INDEX_BUFFER_VIEW view;
        view.BufferLocation = GPUAddress;
        view.SizeInBytes = m_size;
        view.Format = static_cast<DXGI_FORMAT>(m_resourceFormat);

        commandList->IASetIndexBuffer(&view);
    }
}
