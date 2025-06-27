#pragma once

#include "DX12CommandList.h"
#include "DX12Enums.h"

namespace std
{
    enum class byte : unsigned char;
}

namespace MirageAPI::DirectX
{
    public ref class DX12Texture
    {
    internal:
        ID3D12Resource* m_texture = nullptr;
        D3D12_RESOURCE_DESC* m_desc;
        DX12TextureType m_type;
        DXGI_FORMAT m_format;
        D3D12_RESOURCE_STATES m_currentState;

    public:
        DX12Texture(
            int width,
            int height,
            DX12TextureFormat format,
            DX12TextureType type,
            bool generateMipmaps // = false
        );
        ~DX12Texture();
        !DX12Texture();

        void Create();
        void UploadData(std::byte data[], int size, int mipLevel);
        void TransitionState(
            DX12CommandList^ commandList,
            D3D12_RESOURCE_STATES newState
        );

        property ID3D12Resource* NativeResource { ID3D12Resource* get() { return m_texture; } }
        property D3D12_RESOURCE_DESC Description { D3D12_RESOURCE_DESC get() { return *m_desc; } }
        property DXGI_FORMAT Format { DXGI_FORMAT get() { return m_format; } }
        property int Width { int get() { return static_cast<int>(m_desc->Width); } }
        property int Height { int get() { return static_cast<int>(m_desc->Height); } }
    };
}
