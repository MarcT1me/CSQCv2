#pragma once

#include "DX12Descriptor.h"
#include "DX12DescriptorHeapData.h"

namespace MirageAPI::DirectX::Resource
{
    ref class DX12RenderTarget;
}

namespace MirageAPI::DirectX::Descriptors
{
    public ref class DX12DescriptorHeap : public DX12Object<DX12DescriptorHeapData^>
    {
        ID3D12DescriptorHeap* _heap = nullptr;
        UINT _nextFreeIndex = 0;

        Collections::Concurrent::ConcurrentStack<UINT>^ _freeList = gcnew Collections::Concurrent::ConcurrentStack<UINT>();
        QuantumCore::Data::Collections::ConcurrentIdentifierMap<DX12Descriptor^>^ _descriptors = gcnew QuantumCore::Data::Collections::ConcurrentIdentifierMap<DX12Descriptor^>(nullptr);

        ID3D12DescriptorHeap* CreateNativeHeap();
        DX12Descriptor^ CreateDescriptor(Resource::DX12ShaderResource^ resource, UINT index);

    internal:
        D3D12_CPU_DESCRIPTOR_HANDLE CPUHandleForIndex(UINT index);
        D3D12_GPU_DESCRIPTOR_HANDLE GPUHandleForIndex(UINT index);

        property D3D12_CPU_DESCRIPTOR_HANDLE StartCPUHandle
        {
            D3D12_CPU_DESCRIPTOR_HANDLE get() { return _heap->GetCPUDescriptorHandleForHeapStart(); }
        }
        property D3D12_GPU_DESCRIPTOR_HANDLE StartGPUHandle
        {
            D3D12_GPU_DESCRIPTOR_HANDLE get() { return _heap->GetGPUDescriptorHandleForHeapStart(); }
        }

    public:
        // constructors and deconstructors
        DX12DescriptorHeap(
            DX12DescriptorHeapData^ data
        );

        ~DX12DescriptorHeap() { this->!DX12DescriptorHeap(); }
        !DX12DescriptorHeap();

        property ID3D12DescriptorHeap* NativeHeap
        {
            ID3D12DescriptorHeap* get() { return _heap; }
        }
        property Collections::Concurrent::ConcurrentStack<UINT>^ FreeList
        {
            Collections::Concurrent::ConcurrentStack<UINT>^ get() { return _freeList; }
        }
        property QuantumCore::Data::Collections::ConcurrentIdentifierMap<DX12Descriptor^>^ Descriptors
        {
            QuantumCore::Data::Collections::ConcurrentIdentifierMap<DX12Descriptor^>^ get() { return _descriptors; }
        }
        property UINT FreeCount
        {
            UINT get() { return MetaData->_capacity - _nextFreeIndex + _freeList->Count; }
        }

        // Resource::DX12ShaderResource = DX12DescriptorHeapType::SRV, CBV, UAV
        DX12Descriptor^ Allocate(Resource::DX12ShaderResource^ resource);
        DX12Descriptor^ GetResourceDescriptor(Resource::DX12ShaderResource^ resource);
        bool HasResource(Resource::DX12ShaderResource^ resource);
        
        void Free(DX12Descriptor^ descriptor);

        void Reset();
        bool Resize(UINT newCapacity);

        void Validate() override;

        String^ ToString() override;
    };
}
