#include "pch.h"
#include "DX12StructuredBuffer.h"

namespace MirageAPI::DirectX
{
    DX12StructuredBuffer::~DX12StructuredBuffer()
    {
        this->!DX12StructuredBuffer();
    }

    void DX12StructuredBuffer::!DX12StructuredBuffer()
    {
        ReleaseSRV();
        if (m_nativeResource)
        {
            Unmap();
            if (m_nativeResource->Release() == 0)
            {
                m_nativeResource = nullptr;
            }
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

    void DX12StructuredBuffer::CreateSRV()
    {
        if (m_srvIndex != UINT_MAX) return;

        auto srvHeap = DX12Context::GetDescriptorHeap(DX12DescriptorHeapType::CBV_SRV_UAV, 256, true);

        m_srvIndex = srvHeap->Allocate();
        auto device = DX12Context::GetDevice();

        D3D12_CPU_DESCRIPTOR_HANDLE handle = srvHeap->NativeHeap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += m_srvIndex * srvHeap->DescriptorSize;

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = CreateSRVDesc();
        device->CreateShaderResourceView(
            m_nativeResource,
            &desc,
            handle
        );
    }

    void DX12StructuredBuffer::ReleaseSRV()
    {
        if (m_srvIndex == UINT_MAX) return;

        auto srvHeap = DX12Context::GetDescriptorHeap(DX12DescriptorHeapType::CBV_SRV_UAV, 256, true);

        srvHeap->Free(m_srvIndex);
        m_srvIndex = UINT_MAX;
    }
}
