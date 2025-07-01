#pragma once

#include <cstddef>

#include "DX12Buffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Texture : public DX12Buffer
    {
        DX12TextureType m_type;

        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_mipLevels;
        bool m_generateMipmaps;

    public:
        DX12Texture(
            unsigned int width,
            unsigned int height,
            DX12ResourceFormat format,
            DX12TextureType type,
            unsigned int mipLevels,
            DX12ResourceFlags flags
        );

        property DX12TextureType TextureType { DX12TextureType get() { return m_type; } }
        property int Width { int get() { return static_cast<int>(m_width); } }
        property int Height { int get() { return static_cast<int>(m_height); } }
        property bool HasMipmaps { bool get() { return m_mipLevels > 1; } }

        void UploadData(std::byte data[], int width, int height);
        D3D12_SHADER_RESOURCE_VIEW_DESC CreateSRVDesc();

        void GenerateMipmaps(DX12CommandList^ commandList);
    };
}
