#include "pch.h"
#include "DX12DescriptorHeapManager.h"

#include <algorithm>

#include "DX12Context.h"

namespace MirageAPI::DirectX
{
    DX12DescriptorHeapManager::DX12DescriptorHeapManager()
    {
        m_heaps[DX12DescriptorHeapType::CBV_SRV_UAV] =
            gcnew DX12DescriptorHeap(DX12DescriptorHeapType::CBV_SRV_UAV, 256, true);

        m_heaps[DX12DescriptorHeapType::RTV] =
            gcnew DX12DescriptorHeap(DX12DescriptorHeapType::RTV, 16, false);

        m_heaps[DX12DescriptorHeapType::DSV] =
            gcnew DX12DescriptorHeap(DX12DescriptorHeapType::DSV, 8, false);
    }

    DX12DescriptorHeap^ DX12DescriptorHeapManager::GetHeap(
        DX12DescriptorHeapType type,
        UINT minCapacity,
        bool shaderVisible)
    {
        try
        {
            DX12DescriptorHeap^ heap;
            if (!m_heaps->TryGetValue(type, heap) || heap == nullptr)
            {
                heap = gcnew DX12DescriptorHeap(type, minCapacity, shaderVisible);
                m_heaps[type] = heap;
                return heap;
            }

            if (heap->FreeCount < minCapacity)
            {
                UINT newCapacity = std::max(heap->Capacity * 2, minCapacity);
                heap = gcnew DX12DescriptorHeap(type, newCapacity, shaderVisible);
                delete  m_heaps[type];
                m_heaps[type] = heap;
            }

            return heap;
        }
        catch (System::Exception^ ex)
        {
            System::Diagnostics::Debug::WriteLine(
                "Error in GetHeap: " + ex->Message);
            throw;
        }
    }

    void DX12DescriptorHeapManager::ReleaseAll()
    {
        for each (auto pair in m_heaps)
        {
            if (pair.Value != nullptr)
            {
                delete pair.Value;
            }
        }
        m_heaps->Clear();
    }
}
