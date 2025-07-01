#include "pch.h"
#include "DX12VertexBuffer.h"

namespace MirageAPI::DirectX
{
    DX12ResourceConfig GetVertexBufferConfig(
        unsigned int size,
        unsigned int stride
    )
    {
        return {
            DX12ResourceType::VertexBuffer,
            size,
            1,
            stride,
            1,
            1,
            DX12ResourceFormat::Unknown,
            DX12TextureType::None,
            DX12ResourceFlags::None,
            DX12ResourceState::VertexAndConstantBuffer,
            DX12HeapType::Upload
        };
    }

    DX12VertexBuffer::DX12VertexBuffer(
        unsigned int size,
        unsigned int stride
    ) : DX12Buffer(GetVertexBufferConfig(size, stride))
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
