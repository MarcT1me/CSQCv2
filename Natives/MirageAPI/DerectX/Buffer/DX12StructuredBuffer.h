#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12StructuredBuffer : public DX12Buffer
    {
    internal:
        unsigned int m_srvIndex = UINT_MAX;
        DX12DescriptorHeap^ m_srvHeap = nullptr;
        
    public:
        DX12StructuredBuffer(
            unsigned int elementCount,
            unsigned int stride,
            DX12ResourceFlags flags,
            DX12HeapType heapType
        ): DX12Buffer(DX12ResourceConfig::StructuredBufferConfig(elementCount, stride, flags, heapType))
        {
        }

        ~DX12StructuredBuffer();
        !DX12StructuredBuffer();
        
        D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDesc();
        void CreateSRV();
        void ReleaseSRV();
    
        property UINT SRVIndex { UINT get() { return m_srvIndex; } }
        property bool HasSRV { bool get() { return m_srvIndex != UINT_MAX; } }
    };
}
