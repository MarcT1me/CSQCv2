#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12ConstantBuffer : public DX12Buffer
    {
    public:
        DX12ConstantBuffer(unsigned int size);
        void UpdateData(const void* data);
    };
}
