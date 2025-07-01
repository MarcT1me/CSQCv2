#pragma once

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

    protected:
        UINT m_size;
        DX12ResourceFormat m_format;

    public:
        DX12Resource(
            UINT size,
            DX12ResourceFormat format
        ) : m_size(size),
            m_format(format)
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
            ID3D12Resource* get()
            {
                return m_nativeResource;
            }
        }

        property D3D12_GPU_VIRTUAL_ADDRESS GPUAddress
        {
            D3D12_GPU_VIRTUAL_ADDRESS get()
            {
                return m_nativeResource ? m_nativeResource->GetGPUVirtualAddress() : 0;
            }
        }

        property DX12ResourceType ResourceType
        {
            virtual DX12ResourceType get() abstract;
        }

        property DX12ResourceFormat Format
        {
            DX12ResourceFormat get() { return m_format; }
        }

        void* Map();
        void Unmap();
    };
}
