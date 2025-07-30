#include "pch.h"
#include "DX12StructuredBuffer.h"

#include "../../DX12DescriptorHeap.h"

namespace MirageAPI::DirectX::Resource
{
    void DX12StructuredBuffer::!DX12StructuredBuffer()
    {
        Validate();
        
        ReleaseSRV();
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC DX12StructuredBuffer::CreateSRVDesc()
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = static_cast<DXGI_FORMAT>(m_resourceFormat);
        srvDesc.ViewDimension = static_cast<D3D12_SRV_DIMENSION>(m_viewDimension);
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = m_elementCount;
        srvDesc.Buffer.StructureByteStride = m_stride;
        srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
        return srvDesc;
    }

    void DX12StructuredBuffer::CreateSRV()
    {
        if (m_srvIndex != UINT_MAX || m_srvHeap) return;

        m_srvHeap = gcnew DX12DescriptorHeap(DX12DescriptorHeapType::CBV_SRV_UAV, 1, true);

        D3D12_CPU_DESCRIPTOR_HANDLE handle = m_srvHeap->NativeHeap->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += m_srvHeap->Allocate() * m_srvHeap->DescriptorSize;

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = CreateSRVDesc();
        device->CreateShaderResourceView(
            m_nativeResource,
            &desc,
            handle
        );
    }

    void DX12StructuredBuffer::ReleaseSRV()
    {
        if (m_srvIndex == UINT_MAX || !m_srvHeap) return;
        
        m_srvHeap->Free(m_srvIndex);
        m_srvIndex = UINT_MAX;
    }
}
