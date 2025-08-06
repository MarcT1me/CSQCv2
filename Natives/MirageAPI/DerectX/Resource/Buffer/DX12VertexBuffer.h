#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX::Resource
{
    public ref class DX12VertexBuffer : public DX12Buffer
    {
    public:
        DX12VertexBuffer(
            UINT elementCount,
            UINT stride,
            DX12ResourceFlags flags
        ) : DX12Buffer(DX12ResourceConfig::VertexBufferConfig(elementCount, stride, flags))
        {
        }

        void Bind(ID3D12GraphicsCommandList* commandList);
    };
}
