#pragma once

namespace MirageAPI::DirectX
{
    public ref class DX12DescriptorHeap
    {
    internal:
        ID3D12DescriptorHeap* m_heap = nullptr;
        
        unsigned int m_capacity;
        unsigned int m_descriptorSize;
        unsigned int m_nextFreeIndex = 0;
        
        System::Collections::Generic::List<unsigned int>^ m_freeList =
            gcnew System::Collections::Generic::List<unsigned int>();

    public:
        DX12DescriptorHeap(
            DX12DescriptorHeapType heapType,
            unsigned int capacity,
            bool shaderVisible
        );
        ~DX12DescriptorHeap();
        !DX12DescriptorHeap();

        property bool IsValid
        {
            bool get() { return m_heap != nullptr; }
        }

        void Validate()
        {
            if (m_heap == nullptr)
            {
                System::Diagnostics::Debug::WriteLine(
                    "Descriptor heap validation failed: m_heap is null. " +
                    "Capacity: " + m_capacity + ", DescriptorSize: " + m_descriptorSize);

                throw gcnew System::InvalidOperationException(
                    "Descriptor heap is not initialized");
            }

            if (m_descriptorSize == 0)
            {
                throw gcnew System::InvalidOperationException(
                    "Invalid descriptor size (0)");
            }

            if (m_capacity == 0)
            {
                throw gcnew System::InvalidOperationException(
                    "Invalid heap capacity (0)");
            }
        }

        unsigned int Allocate();
        void Free(unsigned int index);
        void Reset();

        property ID3D12DescriptorHeap* NativeHeap { ID3D12DescriptorHeap* get() { return m_heap; } }
        property unsigned int DescriptorSize { unsigned int get() { return m_descriptorSize; } }
        property unsigned int Capacity { unsigned int get() { return m_capacity; } }
        property unsigned int FreeCount
        {
            unsigned int get() { return m_capacity - m_nextFreeIndex + m_freeList->Count; }
        }
    };
}
