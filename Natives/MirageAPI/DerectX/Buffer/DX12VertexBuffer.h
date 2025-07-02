#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12VertexBuffer : public DX12Buffer
    {
    public:
        DX12VertexBuffer(
            unsigned int elementCount,
            unsigned int stride
        );

        void Bind(ID3D12GraphicsCommandList* commandList);
    };
}
