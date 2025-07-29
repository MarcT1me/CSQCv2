#include "pch.h"
#include "DX12Resource.h"

namespace MirageAPI::DirectX::Resource
{
    void DX12Resource::!DX12Resource()
    {
        Validate();

        if (m_nativeResource)
        {
            Unmap();
            if (m_nativeResource->Release() == 0)
                m_nativeResource = nullptr;
        }
    }

    void* DX12Resource::Map()
    {
        Validate();

        // mapping data to range
        void* pData = nullptr;
        D3D12_RANGE range = {0, m_size};

        CheckHResult(
            m_nativeResource->Map(0, &range, &pData),
            "Any err in resource map operation"
        );

        return pData;
    }

    void DX12Resource::Unmap()
    {
        Validate();

        // just unmapping
        D3D12_RANGE range = {0, m_size};
        m_nativeResource->Unmap(0, &range);
    }
}
