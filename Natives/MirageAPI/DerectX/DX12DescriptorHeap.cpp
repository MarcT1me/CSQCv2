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
        HRESULT hr = device->CreateDescriptorHeap(
            &heapDesc,
            IID_PPV_ARGS(&heap)
        );

        if (FAILED(hr))
        {
            System::String^ errorMsg;
            if (hr == E_OUTOFMEMORY)
                errorMsg = "Out of memory while creating descriptor heap";
            else if (hr == E_INVALIDARG)
                errorMsg = "Invalid arguments for descriptor heap creation";
            else
                errorMsg = "Failed to create descriptor heap, HRESULT: " + hr;

            throw gcnew System::Exception(errorMsg);
        }

        if (heap == nullptr)
        {
            throw gcnew System::NullReferenceException(
                "Descriptor heap creation returned null pointer"
            );
        }
        m_heap = heap;

        m_descriptorSize = device->GetDescriptorHandleIncrementSize(
            static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(type)
        );

        if (m_descriptorSize == 0)
        {
            DXSimpleRelease(m_heap);
            throw gcnew System::Exception("Invalid descriptor size (0)");
        }
    }

    DX12DescriptorHeap::!DX12DescriptorHeap()
    {
        Validate();

        DXSimpleRelease(m_heap);
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
            throw gcnew System::InvalidOperationException(
                "Descriptor heap overflow. " +
                "Capacity: " + m_capacity + ", NextIndex: " + m_nextFreeIndex);
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
        CheckNull(
            m_heap,
            gcnew System::InvalidOperationException(
                "Descriptor heap is not initialized: " +
                "Capacity: " + m_capacity + ", DescriptorSize: " + m_descriptorSize
            )
        );
        CheckNull(
            m_descriptorSize,
            gcnew System::InvalidOperationException(
                "Invalid descriptor size: " +
                "Capacity: " + m_capacity + ", DescriptorSize: " + m_descriptorSize
            )
        );
        CheckNull(
            m_capacity,
            gcnew System::InvalidOperationException(
                "Invalid heap capacity: " +
                "Capacity: " + m_capacity + ", DescriptorSize: " + m_descriptorSize
            )
        );
    }
}
