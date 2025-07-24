#include "pch.h"
#include "DX12Resource.h"

namespace MirageAPI::DirectX
{
    void DX12Resource::!DX12Resource()
    {
        Unmap();
        if (m_nativeResource && m_nativeResource->Release() == 0)
            m_nativeResource = nullptr;
    }

    void* DX12Resource::Map()
    {
        Validate();

        // mapping data to range
        void* pData = nullptr;
        D3D12_RANGE range = {0, m_size};
        HRESULT hr = m_nativeResource->Map(0, &range, &pData);
        
        if (SUCCEEDED(hr))
            return pData;

        // checking what's going on
        DX12_CHECK(device, hr, "Any err in resource map operation");

        return nullptr; // if is not an error but whatever not a success
    }

    void DX12Resource::Unmap()
    {
        Validate();

        // just unmapping
        D3D12_RANGE range = {0, m_size};
        m_nativeResource->Unmap(0, &range);
    }
}
