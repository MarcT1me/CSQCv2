#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12VertexBuffer : public DX12Buffer
    {
    public:
        DX12VertexBuffer(UINT size, UINT stride)
            : DX12Buffer(size, stride, DX12BufferType::Vertex)
        {
        }

        void Bind(ID3D12GraphicsCommandList* commandList);
    };
}