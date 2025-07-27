#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX::Resource
{
    public ref class DX12ConstantBuffer : public DX12Buffer
    {
    public:
        DX12ConstantBuffer() : DX12Buffer(DX12ResourceConfig::ConstantBufferConfig())
        {
        }
    };
}
