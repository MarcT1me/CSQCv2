#pragma once
#include "DX12Object.h"

namespace MirageAPI::DirectX
{
    public ref class DX12DescriptorHeap : public DX12Object
    {
        // native
        ID3D12DescriptorHeap* m_heap = nullptr;

        // free queue
        System::Collections::Generic::List<UINT>^ m_freeList =
            gcnew System::Collections::Generic::List<UINT>();

    internal:
        // description
        UINT m_capacity;
        UINT m_descriptorSize;
        UINT m_nextFreeIndex = 0;

    public:
        // constructors and deconstructors
        DX12DescriptorHeap(
            DX12DescriptorHeapType heapType,
            UINT capacity,
            bool shaderVisible
        );

        ~DX12DescriptorHeap() { this->!DX12DescriptorHeap(); }
        !DX12DescriptorHeap();

        property ID3D12DescriptorHeap* NativeHeap
        {
            ID3D12DescriptorHeap* get() { return m_heap; }
        }
        property D3D12_CPU_DESCRIPTOR_HANDLE StartCPUHandle
        {
            D3D12_CPU_DESCRIPTOR_HANDLE get() { return m_heap->GetCPUDescriptorHandleForHeapStart(); }
        }
        property UINT DescriptorSize
        {
            UINT get() { return m_descriptorSize; }
        }
        property UINT Capacity
        {
            UINT get() { return m_capacity; }
        }
        property UINT FreeCount
        {
            UINT get() { return m_capacity - m_nextFreeIndex + m_freeList->Count; }
        }

        UINT Allocate();
        void Free(UINT index);
        void Reset();

        void Validate() override
        {
            if (m_heap == nullptr)
            {
                throw gcnew System::InvalidOperationException(
                    "Descriptor heap is not initialized: " +
                    "Capacity: " + m_capacity + ", DescriptorSize: " + m_descriptorSize
                );
            }

            if (m_descriptorSize == 0)
            {
                throw gcnew System::InvalidOperationException(
                    "Invalid descriptor size: " +
                    "Capacity: " + m_capacity + ", DescriptorSize: " + m_descriptorSize
                );
            }

            if (m_capacity == 0)
            {
                throw gcnew System::InvalidOperationException(
                    "Invalid heap capacity: " +
                    "Capacity: " + m_capacity + ", DescriptorSize: " + m_descriptorSize
                );
            }
        }
    };
}
