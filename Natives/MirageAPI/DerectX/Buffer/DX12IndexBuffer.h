#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12IndexBuffer : public DX12Buffer
    {
    public:
        DX12IndexBuffer(DX12ResourceConfig config);
        
        void Bind(ID3D12GraphicsCommandList* commandList);
    };
}