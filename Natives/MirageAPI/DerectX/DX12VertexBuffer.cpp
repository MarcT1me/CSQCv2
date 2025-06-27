#include "pch.h"
#include "DX12VertexBuffer.h"

namespace MirageAPI::DirectX
{
    void DX12VertexBuffer::Bind(ID3D12GraphicsCommandList* commandList)
    {
        D3D12_VERTEX_BUFFER_VIEW view;
        view.BufferLocation = GPUAddress;
        view.SizeInBytes = Size;
        view.StrideInBytes = Stride;
        
        commandList->IASetVertexBuffers(0, 1, &view);
    }
}