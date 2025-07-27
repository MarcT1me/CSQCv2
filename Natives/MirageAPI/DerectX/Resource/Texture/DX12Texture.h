#pragma once

// Mirage ecosystem
#include "../Buffer/DX12Buffer.h"

namespace MirageAPI::DirectX
{
    ref class DX12DescriptorHeap;
}

// texture
namespace MirageAPI::DirectX::Resource
{
    public ref class DX12Texture : public DX12Resource
    {
    internal:
        // description
        DX12TextureType m_textureType;
        UINT m_width;
        UINT m_height;
        UINT m_mipLevels;
        // heap
        UINT m_srvDescriptorIndex = UINT_MAX;
        DX12DescriptorHeap^ m_srvHeap = nullptr;

    public:
        // constructors and deconstructors
        DX12Texture(
            DX12ResourceConfig config
        );

        ~DX12Texture() { this->!DX12Texture(); }
        !DX12Texture();

        // other properties
        property DX12TextureType TextureType
        {
            DX12TextureType get() { return m_textureType; }
        }
        property UINT Width
        {
            UINT get() { return m_width; }
        }
        property UINT Height
        {
            UINT get() { return m_height; }
        }
        property UINT MipLevels
        {
            UINT get() { return m_mipLevels; }
        }
        property DX12DescriptorHeap^ SRVHeap
        {
            DX12DescriptorHeap^ get() { return m_srvHeap; }
            void set(DX12DescriptorHeap^ value) { m_srvHeap = value; }
        }
        property D3D12_CPU_DESCRIPTOR_HANDLE SRVHandle
        {
            D3D12_CPU_DESCRIPTOR_HANDLE get();
        }

        // texture operations
        virtual void UploadData(array<System::Byte>^ data);

        const D3D12_SHADER_RESOURCE_VIEW_DESC* CreateSRVDesc();
        void CreateSRV();
        void ReleaseSRV();

        property UINT SRVIndex { UINT get() { return m_srvDescriptorIndex; } }
        property bool HasSRV { bool get() { return m_srvDescriptorIndex != UINT_MAX; } }
    };
}
