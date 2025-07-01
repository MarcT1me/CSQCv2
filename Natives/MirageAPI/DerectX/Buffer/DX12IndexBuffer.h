#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12IndexBuffer : public DX12Buffer
    {
    public:
        DX12IndexBuffer(
            unsigned int size,
            DX12ResourceFormat format
        );

        void Bind(ID3D12GraphicsCommandList* commandList);
    };
}
