#pragma once

// Mirage ecosystem
#include "../DX12Object.h"
#include "DX12ResourceConfig.h"

// resource
namespace MirageAPI::DirectX
{
    public ref class DX12Resource abstract : public DX12Object
    {
    protected:
        // native
        ID3D12Resource* m_nativeResource = nullptr;

    internal:
        // description
        DX12ResourceState m_currentState;
        DX12ResourceType m_resourceType;
        DX12ResourceFormat m_resourceFormat;
        UINT m_size;

    public:
        // constructors and deconstructors
        DX12Resource(
            DX12ResourceConfig config
        ) : m_currentState(config.InitialState),
            m_resourceType(config.Type),
            m_resourceFormat(config.Format),
            m_size(config.Width * config.Height * config.Stride)
        {
        }

        ~DX12Resource() { this->!DX12Resource(); }
        !DX12Resource();

        // native getters
        property ID3D12Resource* NativeResource
        {
            ID3D12Resource* get() { return m_nativeResource; }
        }
        property D3D12_GPU_VIRTUAL_ADDRESS GPUAddress
        {
            D3D12_GPU_VIRTUAL_ADDRESS get()
            {
                Validate();
                return m_nativeResource->GetGPUVirtualAddress();
            }
        }
        // other getters
        property DX12ResourceState CurrentState
        {
            DX12ResourceState get() { return m_currentState; }
        }
        virtual property DX12ResourceType ResourceType
        {
            DX12ResourceType get() { return m_resourceType; }
        }
        property DX12ResourceFormat ResourceFormat
        {
            DX12ResourceFormat get() { return m_resourceFormat; }
        }
        property UINT Size
        {
            UINT get() { return m_size; }
        }

        // resource operations
        void* Map();
        void Unmap();
    };
}
