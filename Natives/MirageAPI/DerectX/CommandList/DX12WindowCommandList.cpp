#include "pch.h"
#include "DX12WindowCommandList.h"

#include "../DX12DescriptorHeap.h"

namespace MirageAPI::DirectX
{
    DX12WindowCommandList::DX12WindowCommandList() : DX12CommandList(DX12CommandListType::Direct)
    {
        DescriptorHeap = gcnew DX12DescriptorHeap(DX12DescriptorHeapType::CBV_SRV_UAV, 256, false);
    }
}
