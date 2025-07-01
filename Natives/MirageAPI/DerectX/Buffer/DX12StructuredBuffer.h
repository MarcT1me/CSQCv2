#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12StructuredBuffer : public DX12Buffer
    {
    public:
        DX12StructuredBuffer(
            unsigned int elementCount,
            unsigned int strid,
            DX12HeapType heapType,
            DX12ResourceFlags flags
        );

        void UpdateData(array<System::Byte>^ data);
        D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDesc();

        property unsigned int ElementCount { unsigned int get() { return m_elementCount; } }
    };
}
