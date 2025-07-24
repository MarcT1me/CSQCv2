#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12VertexBuffer : public DX12Buffer
    {
    public:
        DX12VertexBuffer(
            UINT elementCount,
            UINT stride
        ) : DX12Buffer(DX12ResourceConfig::VertexBufferConfig(elementCount, stride))
        {
        }

        void Bind(ID3D12GraphicsCommandList* commandList);
    };
}
