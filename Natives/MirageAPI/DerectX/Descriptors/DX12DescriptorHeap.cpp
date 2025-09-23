#include "pch.h"
#include "DX12DescriptorHeap.h"

#include "../Resource/DX12ShaderResource.h"

namespace MirageAPI::DirectX::Descriptors
{
    DX12DescriptorHeap::DX12DescriptorHeap(
        DX12DescriptorHeapData^ data
    ) : DX12Object(data)
    {
        _heap = CreateNativeHeap();
        MetaData->_descriptorSize = device->GetDescriptorHandleIncrementSize(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(MetaData->_type));
        Validate();
    }

    DX12DescriptorHeap::!DX12DescriptorHeap()
    {
        SimpleRelease(_heap);
    }

    ID3D12DescriptorHeap* DX12DescriptorHeap::CreateNativeHeap()
    {
        // description
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(MetaData->_type);
        heapDesc.NumDescriptors = MetaData->_capacity;
        heapDesc.Flags = static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS>(MetaData->_shaderVisible);

        // creation
        ID3D12DescriptorHeap* heap;
        CheckHResult(
            device->CreateDescriptorHeap(
                &heapDesc,
                IID_PPV_ARGS(&heap)
            ),
            "CreateDescriptorHeap failed"
        );

        return heap;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::CPUHandleForIndex(UINT index)
    {
        D3D12_CPU_DESCRIPTOR_HANDLE handle = StartCPUHandle;
        handle.ptr += index * MetaData->_descriptorSize;
        return handle;
    }

    D3D12_GPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::GPUHandleForIndex(UINT index)
    {
        D3D12_GPU_DESCRIPTOR_HANDLE handle = StartGPUHandle;
        handle.ptr += index * MetaData->_descriptorSize;
        return handle;
    }

    DX12Descriptor^ DX12DescriptorHeap::Allocate(Resource::DX12ShaderResource^ resource)
    {
        Validate();

        // if any free index is existed
        UINT index;
        if (_freeList->TryPop(index))
            return CreateDescriptor(resource, index);

        // try to resize heap
        if (_nextFreeIndex >= MetaData->_capacity)
        {
            if (Resize(MetaData->_capacity * 2))
            {
                return Allocate(resource);
            }

            // if not resized throw exception
            throw gcnew DXException(
                CSFormat(
                    "Descriptor heap overflow. Capacity: {0}, Index: {1}",
                    MetaData->_capacity, _nextFreeIndex
                )
            );
        }

        // return increased
        return CreateDescriptor(resource, _nextFreeIndex++);
    }

    DX12Descriptor^ DX12DescriptorHeap::CreateDescriptor(Resource::DX12ShaderResource^ resource, UINT index)
    {
        DX12Descriptor^ descriptor = gcnew DX12Descriptor(resource, this, index);
        _descriptors[resource->Id] = descriptor;
        return descriptor;
    }

    void DX12DescriptorHeap::Free(DX12Descriptor^ descriptor)
    {
        // just move descriptor index in free list
        if (!_freeList->TryPeek(descriptor->_heapIndex))
        {
            _freeList->Push(descriptor->_heapIndex);
            descriptor->_isDisposed = true;
        }
    }

    bool DX12DescriptorHeap::HasResource(Resource::DX12ShaderResource^ resource)
    {
        return _descriptors->ContainsKey(resource->Id);
    }

    DX12Descriptor^ DX12DescriptorHeap::GetResourceDescriptor(Resource::DX12ShaderResource^ resource)
    {
        DX12Descriptor^ descriptor;
        if (!_descriptors->TryGetValue(resource->Id, descriptor)) return nullptr;
        return descriptor;
    }

    void DX12DescriptorHeap::Reset()
    {
        // deleting all descriptors
        _descriptors->Clear();
        // clear fields
        _nextFreeIndex = 0;
        _freeList->Clear();
    }

    bool DX12DescriptorHeap::Resize(UINT newCapacity)
    {
        // checking base cases
        if (!MetaData->AutoAllocate)
            return false;
        if (newCapacity <= MetaData->_capacity)
            return true;

        Validate();

        // trying to resize
        try
        {
            auto newHeap = CreateNativeHeap();
            device->CopyDescriptorsSimple(
                _nextFreeIndex,
                newHeap->GetCPUDescriptorHandleForHeapStart(),
                _heap->GetCPUDescriptorHandleForHeapStart(),
                static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(MetaData->_type)
            );
            SimpleRelease(_heap);
            _heap = newHeap;
            MetaData->_capacity = newCapacity;

            Validate();
            return true;
        }
        catch (QuantumFailure^ e)
        {
            QLog(Error, "Failed to resize heap " + Id + " (inner exception): " + e->Message);
            return false;
        }
        catch (Exception^ ex)
        {
            QLog(Error, "Failed to resize heap " + Id + " (exception): " + ex->Message);
            return false;
        }
        catch (std::exception* e)
        {
            QLog(Error, "Failed to resize heap " + Id + " (unknown error): " + gcnew String(e->what()));
            return false;
        }
    }

    void DX12DescriptorHeap::Validate()
    {
        ThrowIfNull(
            _heap,
            CSFormat(
                "Descriptor heap is not initialized. Capacity: {0}, DescriptorSize: {1}",
                MetaData->_capacity, MetaData->_descriptorSize
            )
        );
        MetaData->Validate();
    }

    String^ DX12DescriptorHeap::ToString()
    {
        return CSFormat(
            "DX12DescriptorHeap<{0}>(next Free: {1}, Free size: {2}, allocated: {3})",
            Id, _nextFreeIndex, _freeList->Count, _descriptors->Count
        );
    }
}
