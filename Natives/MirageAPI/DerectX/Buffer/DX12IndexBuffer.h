#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12IndexBuffer : public DX12Buffer
    {
    public:
        DX12IndexBuffer(
            UINT elementCount,
            DX12ResourceFormat format
        ) : DX12Buffer(DX12ResourceConfig::IndexBufferConfig(elementCount, format))
        {
        }

        void Bind(ID3D12GraphicsCommandList* commandList);
    };
}
