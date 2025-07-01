#include "pch.h"
#include "DX12StructuredBuffer.h"

namespace MirageAPI::DirectX
{
    DX12ResourceConfig GetStructuredBufferConfig(
        unsigned int
        elementCount,
        unsigned int stride,
        DX12HeapType heapType,
        DX12ResourceFlags flags
    )
    {
        DX12ResourceConfig config;
        config.Type = DX12ResourceType::StructuredBuffer;
        config.Width = static_cast<UINT64>(elementCount) * stride;
        config.Stride = stride;
        config.HeapType = heapType;
        config.InitialState = DX12ResourceState::Common;
        config.Flags = flags;
        return config;
    }

    DX12StructuredBuffer::DX12StructuredBuffer(
        unsigned int elementCount,
        unsigned int stride,
        DX12HeapType heapType,
        DX12ResourceFlags flags
    ): DX12Buffer(GetStructuredBufferConfig(elementCount, stride, heapType, flags))
    {
    }

    void DX12StructuredBuffer::UpdateData(array<System::Byte>^ data)
    {
        if (!m_nativeResource || data->Length != m_size) return;

        if (void* pData = this->Map())
        {
            pin_ptr<System::Byte> pinData = &data[0];
            memcpy(pData, pinData, m_size);
            Unmap();
        }
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC DX12StructuredBuffer::CreateSRVDesc()
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = m_elementCount;
        srvDesc.Buffer.StructureByteStride = m_stride;
        srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
        return srvDesc;
    }
}
