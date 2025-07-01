#include "pch.h"
#include "DX12IndexBuffer.h"

#include "../DX12Helpers.h"

namespace MirageAPI::DirectX
{
    DX12ResourceConfig GetIndexBufferConfig(
        unsigned int size,
        DX12ResourceFormat format
    )
    {
        if (format != DX12ResourceFormat::R16_UINT &&
            format != DX12ResourceFormat::R32_UINT)
        {
            throw gcnew System::ArgumentException("Invalid index buffer format");
        }

        return {
            DX12ResourceType::IndexBuffer,
            size,
            1,
            GetResourceFormatSize(format),
            1,
            1,
            format,
            DX12TextureType::None,
            DX12ResourceFlags::None,
            DX12ResourceState::IndexBuffer,
            DX12HeapType::Upload
        };
    }

    DX12IndexBuffer::DX12IndexBuffer(
        unsigned int size,
        DX12ResourceFormat format
    ) : DX12Buffer(GetIndexBufferConfig(size, format))
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
