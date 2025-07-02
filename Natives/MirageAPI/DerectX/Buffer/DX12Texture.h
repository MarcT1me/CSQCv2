#pragma once

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Texture : public DX12Resource
    {
    internal:
        DX12TextureType m_textureType;
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_mipLevels;
        unsigned int m_srvIndex = UINT_MAX;
        DX12DescriptorHeap^ m_srvHeap = nullptr;

    public:
        DX12Texture(
            DX12ResourceConfig config
        );

        ~DX12Texture();
        !DX12Texture();

        void TransitionState(
            DX12CommandList^ commandList,
            DX12ResourceState newState
        ) override;

        property DX12TextureType TextureType { DX12TextureType get() { return m_textureType; } }
        property int Width { int get() { return static_cast<int>(m_width); } }
        property int Height { int get() { return static_cast<int>(m_height); } }
        property bool HasMipmaps { bool get() { return m_mipLevels > 1; } }

        void UploadData(array<System::Byte>^ data);

        D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDesc();
        void CreateSRV();
        void ReleaseSRV();

        property UINT SRVIndex { UINT get() { return m_srvIndex; } }
        property bool HasSRV { bool get() { return m_srvIndex != UINT_MAX; } }
    };
}
