#include "pch.h"
#include "DX12DescriptorHeap.h"

namespace MirageAPI::DirectX
{
    DX12DescriptorHeap::DX12DescriptorHeap(
        DX12DescriptorHeapType type,
        UINT capacity,
        bool shaderVisible
    ) : m_capacity(capacity)
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(type);
        heapDesc.NumDescriptors = capacity;
        heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        ID3D12DescriptorHeap* heap;
        CheckHResult(
            device->CreateDescriptorHeap(
                &heapDesc,
                IID_PPV_ARGS(&heap)
            ),
            "CreateDescriptorHeap failed"
        );
        m_heap = heap;

        m_descriptorSize = device->GetDescriptorHandleIncrementSize(
            static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(type)
        );

        CheckNull(m_descriptorSize)
        {
            SimpleRelease(m_heap);
            throw gcnew DXException("Invalid descriptor size (0)");
        }
    }

    DX12DescriptorHeap::!DX12DescriptorHeap()
    {
        Validate();

        SimpleRelease(m_heap);
    }

    UINT DX12DescriptorHeap::Allocate()
    {
        Validate();

        if (m_freeList->Count > 0)
        {
            UINT index = m_freeList[0];
            m_freeList->RemoveAt(0);
            return index;
        }

        if (m_nextFreeIndex >= m_capacity)
        {
            throw gcnew DXException(
                CSFormat(
                    "Descriptor heap overflow. Capacity: {0}, Index: {1}",
                    m_capacity, m_nextFreeIndex
                )
            );
        }

        return m_nextFreeIndex++;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::IndexCPUHandle(UINT index)
    {
        Validate();

        D3D12_CPU_DESCRIPTOR_HANDLE handle = StartCPUHandle;
        handle.ptr += index * m_descriptorSize;
        return handle;
    }

    D3D12_GPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::IndexGPUHandle(UINT index)
    {
        Validate();

        D3D12_GPU_DESCRIPTOR_HANDLE handle = StartGPUHandle;
        handle.ptr += index * m_descriptorSize;
        return handle;
    }

    void DX12DescriptorHeap::Free(UINT index)
    {
        if (index < m_capacity)
            m_freeList->Add(index);
    }

    void DX12DescriptorHeap::Reset()
    {
        m_nextFreeIndex = 0;
        m_freeList->Clear();
    }

    void DX12DescriptorHeap::Validate()
    {
        ThrowIfNull(
            m_heap,
            CSFormat(
                "Descriptor heap is not initialized. Capacity: {0}, DescriptorSize: {1}",
                m_capacity, m_descriptorSize
            )
        );
        ThrowIfNull(
            m_heap,
            CSFormat(
                "Descriptor heap is not initialized. Capacity: {0}, DescriptorSize: {1}",
                m_capacity, m_descriptorSize
            )
        );
        ThrowIfNull(
            m_descriptorSize,
            CSFormat(
                "Invalid descriptor size. Capacity: {0}, DescriptorSize: {1}",
                m_capacity, m_descriptorSize
            )
        );
        ThrowIfNull(
            m_capacity,
            CSFormat(
                "Invalid heap capacity. Capacity: {0}, DescriptorSize: {1}",
                m_capacity, m_descriptorSize
            )
        );
    }
}
