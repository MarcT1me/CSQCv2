#include "pch.h"
#include "DX12Resource.h"

namespace MirageAPI::DirectX
{
    DX12Resource::~DX12Resource()
    {
        this->!DX12Resource();
    }

    void DX12Resource::!DX12Resource()
    {
        if (m_nativeResource)
        {
            Unmap();
            if (m_nativeResource->Release() == 0)
            {
                m_nativeResource = nullptr;
            }
        }
    }

    void* DX12Resource::Map()
    {
        if (!m_nativeResource) return nullptr;

        void* pData = nullptr;
        D3D12_RANGE range = {0, m_size};
        if (SUCCEEDED(m_nativeResource->Map(0, &range, &pData)))
            return pData;

        return nullptr;
    }

    void DX12Resource::Unmap()
    {
        if (m_nativeResource)
        {
            D3D12_RANGE range = {0, m_size};
            m_nativeResource->Unmap(0, &range);
        }
    }
}
