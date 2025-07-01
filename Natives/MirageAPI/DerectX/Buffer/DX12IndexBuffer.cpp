#include "pch.h"
#include "DX12IndexBuffer.h"

namespace MirageAPI::DirectX
{
    DX12IndexBuffer::DX12IndexBuffer(DX12ResourceConfig config)
        : DX12Buffer(config)
    {
        // Проверяем, что формат подходит для индексного буфера
        if (config.Format != DX12ResourceFormat::R16_UINT &&
            config.Format != DX12ResourceFormat::R32_UINT)
        {
            throw gcnew System::ArgumentException("Invalid index buffer format");
        }
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
