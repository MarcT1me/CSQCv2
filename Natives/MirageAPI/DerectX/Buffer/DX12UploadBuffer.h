#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12UploadBuffer : public DX12Buffer
    {
    public:
        DX12UploadBuffer(UINT size) : DX12Buffer(DX12ResourceConfig::UploadBufferConfig(size))
        {
        }
    };
}
