#pragma once

#include "..\Resource\DX12Resource.h"
#include "..\Resource\DX12ResourceConfig.h"

namespace std
{
    enum class byte : unsigned char;
}

namespace MirageAPI::DirectX
{
    public ref class DX12Texture : public DX12Resource
    {
        DX12TextureType m_type;

        UINT m_width;
        UINT m_height;
        UINT m_mipLevels;
        bool m_generateMipmaps;

    public:
        DX12Texture(DX12ResourceConfig config);

        void TransitionState(
            DX12CommandList^ commandList,
            DX12ResourceState newState
        ) override;

        virtual property DX12ResourceType ResourceType {
            DX12ResourceType get() override { return DX12ResourceType::Texture; }
        }

        property DX12TextureType TextureType {
            DX12TextureType get() { return m_type; }
        }
        
        void UploadData(std::byte data[], int width, int height);
        void GenerateMipmaps(DX12CommandList^ commandList);
        property int Width { int get() { return static_cast<int>(m_width); } }
        property int Height { int get() { return static_cast<int>(m_height); } }
        property bool HasMipmaps { bool get() { return m_mipLevels > 1; } }
    };
}
