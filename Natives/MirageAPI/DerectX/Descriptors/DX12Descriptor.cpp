#include "pch.h"

#include "DX12Descriptor.h"
#include "DX12DescriptorHeap.h"
#include "../Resource/DX12ShaderResource.h"

namespace MirageAPI::DirectX::Descriptors
{
    DX12Descriptor::DX12Descriptor(
        DirectX::Resource::DX12ShaderResource^ resource,
        DX12DescriptorHeap^ heap,
        UINT index
    ) : DX12ObjectData(resource->Id),
        _resource(resource),
        _heap(heap),
        _heapIndex(index)
    {
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12Descriptor::_CPUHandle::get()
    {
        return _heap->CPUHandleForIndex(_heapIndex);
    }

    D3D12_GPU_DESCRIPTOR_HANDLE DX12Descriptor::_GPUHandle::get()
    {
        return _heap->GPUHandleForIndex(_heapIndex);
    }

    String^ DX12Descriptor::ToString()
    {
        return CSFormat(
            "DX12Descriptor<{0}>(index: {1}, heap: {2})",
            Identifier, _heapIndex, _heap->ToString()
        );
    }
}
