#pragma once

#include "../DX12ObjectData.h"

namespace MirageAPI::DirectX::Descriptors
{
    ref class DX12DescriptorHeap;
}

namespace MirageAPI::DirectX::Resource
{
    ref class DX12ShaderResource;
}

namespace MirageAPI::DirectX::Descriptors
{
    public ref struct DX12Descriptor : public DX12ObjectData
    {
    internal:
        Resource::DX12ShaderResource^ _resource;
        DX12DescriptorHeap^ _heap;
        UINT _heapIndex;
        bool _isDisposed;

        property D3D12_CPU_DESCRIPTOR_HANDLE _CPUHandle
        {
            D3D12_CPU_DESCRIPTOR_HANDLE get();
        }
        property D3D12_GPU_DESCRIPTOR_HANDLE _GPUHandle
        {
            D3D12_GPU_DESCRIPTOR_HANDLE get();
        }

    public:
        DX12Descriptor(Resource::DX12ShaderResource^ resource, DX12DescriptorHeap^ heap, UINT index);

        property Resource::DX12ShaderResource^ Resource
        {
            DirectX::Resource::DX12ShaderResource^ get() { return _resource; }
        }
        property DX12DescriptorHeap^ Heap
        {
            DX12DescriptorHeap^ get() { return _heap; }
        }
        property UINT HeapIndex
        {
            UINT get() { return _heapIndex; }
        }
        property bool IsDisposed
        {
            bool get() { return _isDisposed; }
        }

        String^ ToString() override;
    };
}
