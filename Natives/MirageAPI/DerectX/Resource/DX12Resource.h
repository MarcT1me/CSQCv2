#pragma once
#include "DX12ResourceConfig.h"

namespace MirageAPI::DirectX
{
    ref class DX12CommandList;
    enum class DX12ResourceState;
}

namespace MirageAPI::DirectX
{
    public ref class DX12Resource abstract
    {
    internal:
        ID3D12Resource* m_nativeResource = nullptr;
        DX12ResourceState m_currentState;
        DX12ResourceType m_resourceType;
        DX12ResourceFormat m_resourceFormat;
        unsigned int m_size;

    public:
        DX12Resource(
            DX12ResourceConfig config
        ) : m_currentState(config.InitialState),
            m_resourceType(config.Type),
            m_resourceFormat(config.Format),
            m_size(config.Width * config.Width * config.Stride)
        {
        }

        virtual ~DX12Resource();
        !DX12Resource();

        virtual void TransitionState(
            DX12CommandList^ commandList,
            DX12ResourceState newState
        ) abstract;

        property ID3D12Resource* NativeResource
        {
            ID3D12Resource* get() { return m_nativeResource; }
        }

        property D3D12_GPU_VIRTUAL_ADDRESS GPUAddress
        {
            D3D12_GPU_VIRTUAL_ADDRESS get() { return m_nativeResource->GetGPUVirtualAddress(); }
        }

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

        property unsigned int Size
        {
            unsigned int get() { return m_size; }
        }

        void* Map();
        void Unmap();
    };
}
