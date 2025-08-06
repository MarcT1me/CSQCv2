#pragma once

namespace MirageAPI::DirectX
{
    public ref class DX12DescriptorHeap : public DX12Object
    {
        // native
        ID3D12DescriptorHeap* m_heap = nullptr;

        // free queue
        CSList<UINT>^ m_freeList = gcnew CSList<UINT>();

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
        property D3D12_GPU_DESCRIPTOR_HANDLE StartGPUHandle
        {
            D3D12_GPU_DESCRIPTOR_HANDLE get() { return m_heap->GetGPUDescriptorHandleForHeapStart(); }
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
        D3D12_CPU_DESCRIPTOR_HANDLE IndexCPUHandle(UINT index);
        D3D12_GPU_DESCRIPTOR_HANDLE IndexGPUHandle(UINT index);
        void Free(UINT index);
        void Reset();

        void Validate() override;
    };
}
