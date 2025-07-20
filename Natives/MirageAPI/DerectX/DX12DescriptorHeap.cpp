#include "pch.h"
#include "DX12DescriptorHeap.h"

#include "DX12Context.h"

namespace MirageAPI::DirectX
{
    DX12DescriptorHeap::DX12DescriptorHeap(
        DX12DescriptorHeapType type,
        UINT capacity,
        bool shaderVisible
    ) : m_capacity(capacity)
    {
        auto device = GetContextDevice();

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
                errorMsg = "Failed to create descriptor heap, HRESULT: 0x" + hr;

            throw gcnew System::Exception(errorMsg);
        }

        if (heap == nullptr)
        {
            throw gcnew System::NullReferenceException(
                "Descriptor heap creation returned null pointer");
        }

        m_heap = heap;
        m_descriptorSize = device->GetDescriptorHandleIncrementSize(
            static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(type)
        );

        if (m_descriptorSize == 0)
        {
            m_heap->Release();
            m_heap = nullptr;
            throw gcnew System::Exception("Invalid descriptor size (0)");
        }
    }

    DX12DescriptorHeap::~DX12DescriptorHeap() { this->!DX12DescriptorHeap(); }

    DX12DescriptorHeap::!DX12DescriptorHeap()
    {
        if (m_heap)
        {
            m_heap->Release();
            m_heap = nullptr;
        }

        m_capacity = 0;
        m_descriptorSize = 0;
        m_nextFreeIndex = 0;
        m_freeList->Clear();
    }

    UINT DX12DescriptorHeap::Allocate()
    {
        if (!m_heap || m_descriptorSize == 0)
        {
            throw gcnew System::InvalidOperationException("Descriptor heap not initialized");
        }

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
                "Descriptor heap overflow. Capacity: " + m_capacity +
                ", NextIndex: " + m_nextFreeIndex);
        }

        return m_nextFreeIndex++;
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
}
