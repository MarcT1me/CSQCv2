#pragma once
#include "DX12DescriptorHeap.h"

namespace MirageAPI::DirectX
{
    public ref class DX12DescriptorHeapManager
    {
        static DX12DescriptorHeapManager^ s_instance = nullptr;
        System::Collections::Generic::Dictionary<DX12DescriptorHeapType, DX12DescriptorHeap^>^ m_heaps =
            gcnew System::Collections::Generic::Dictionary<DX12DescriptorHeapType, DX12DescriptorHeap^>();

        DX12DescriptorHeapManager();

    public:
        static property DX12DescriptorHeapManager^ Instance
        {
            DX12DescriptorHeapManager^ get()
            {
                if (s_instance == nullptr)
                    s_instance = gcnew DX12DescriptorHeapManager();
                return s_instance;
            }
        }

        DX12DescriptorHeap^ GetHeap(
            DX12DescriptorHeapType type,
            UINT minCapacity,
            bool shaderVisible);

        void ReleaseAll();
    };
}
