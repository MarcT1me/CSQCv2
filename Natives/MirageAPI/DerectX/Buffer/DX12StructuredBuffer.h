#pragma once

// Mirage ecosystem
#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    ref class DX12DescriptorHeap;
}

// buffer
namespace MirageAPI::DirectX
{
    public ref class DX12StructuredBuffer : public DX12Buffer
    {
    internal:
        // heap
        unsigned int m_srvIndex = UINT_MAX;
        DX12DescriptorHeap^ m_srvHeap = nullptr;

    public:
        // constructors and deconstructors
        DX12StructuredBuffer(
            unsigned int elementCount,
            unsigned int stride,
            DX12ResourceFlags flags,
            DX12HeapType heapType
        ): DX12Buffer(DX12ResourceConfig::StructuredBufferConfig(elementCount, stride, flags, heapType))
        {
        }

        ~DX12StructuredBuffer() { this->!DX12StructuredBuffer(); }
        !DX12StructuredBuffer();

        // other getters
        property UINT SRVIndex { UINT get() { return m_srvIndex; } }
        property bool HasSRV { bool get() { return m_srvIndex != UINT_MAX; } }

        // buffer operations
        D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDesc();
        void CreateSRV();
        void ReleaseSRV();
    };
}
