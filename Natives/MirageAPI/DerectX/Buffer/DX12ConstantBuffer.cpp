#include "pch.h"
#include "DX12ConstantBuffer.h"

#include <algorithm>

namespace MirageAPI::DirectX
{
    DX12ResourceConfig GetConstantBufferConfig(unsigned int size)
    {
        DX12ResourceConfig config;
        config.Type = DX12ResourceType::ConstantBuffer;
        config.Width = size + 255 & ~255;
        config.HeapType = DX12HeapType::Upload;
        config.InitialState = DX12ResourceState::GenericRead;
        return config;
    }

    DX12ConstantBuffer::DX12ConstantBuffer(
        unsigned int size
    ) : DX12Buffer(GetConstantBufferConfig(size))
    {
    }

    void DX12ConstantBuffer::UpdateData(const void* data)
    {
        if (void* pData = this->Map())
        {
            memcpy(pData, data, m_size);
            this->Unmap();
        }
    }
}
