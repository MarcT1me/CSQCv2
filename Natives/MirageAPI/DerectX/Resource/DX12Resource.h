#pragma once

// Mirage ecosystem
#include "DX12ResourceConfig.h"

namespace MirageAPI::DirectX::Command
{
    ref class DX12CommandList;
}

// resource
namespace MirageAPI::DirectX::Resource
{
    public ref class DX12Resource abstract : public DX12Object<DX12ResourceConfig^>
    {
    protected:
        // native
        ID3D12Resource* _nativeResource;
        // descriptions
        DX12ResourceState _currentState;
        UINT _size;
        int _refCount;

    public:
        // constructors and deconstructors
        DX12Resource(
            DX12ResourceConfig^ config
        ) : DX12Object(config),
            _size(config->Width * config->Height * config->Stride)
        {
        }

        ~DX12Resource() { this->!DX12Resource(); }
        !DX12Resource();

        // native getters
        property ID3D12Resource* NativeResource
        {
            ID3D12Resource* get() { return _nativeResource; }
        }
        property D3D12_GPU_VIRTUAL_ADDRESS GPUAddress
        {
            D3D12_GPU_VIRTUAL_ADDRESS get()
            {
                Validate();
                return _nativeResource->GetGPUVirtualAddress();
            }
        }
        // other getters
        property DX12ResourceState CurrentState
        {
            DX12ResourceState get() { return _currentState; }
        }
        property UINT Size
        {
            UINT get() { return _size; }
        }
        property int RefCount
        {
            int get() { return _refCount; }
        }

        void AddRef();
        void Release();

        // resource operations
        void TransitionState(
            Command::DX12CommandList^ commandList,
            DX12ResourceState newState
        );
        void* Map();
        void Unmap();
    };
}
