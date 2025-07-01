#pragma once

namespace MirageAPI::DirectX
{
    public value struct DX12ResourceConfig
    {
        DX12ResourceType Type;

        UINT64 Width;
        UINT Height;
        UINT Stride;

        UINT Depth;
        UINT MipLevels;

        DX12ResourceFormat Format;
        DX12TextureType TextureType;
        
        DX12ResourceFlags Flags;
        DX12ResourceState InitialState;
        
        DX12HeapType HeapType;

        static property DX12ResourceConfig DefaultVertexBuffer
        {
            DX12ResourceConfig get()
            {
                return {
                    DX12ResourceType::Buffer,
                    1024, 1,
                    sizeof(float) * 3,
                    1, 1,
                    DX12ResourceFormat::Unknown,
                    DX12TextureType::None,
                    DX12ResourceFlags::None,
                    DX12ResourceState::VertexAndConstantBuffer,
                    DX12HeapType::Upload
                };
            }
        }

        static property DX12ResourceConfig DefaultRenderTarget
        {
            DX12ResourceConfig get()
            {
                return {
                    DX12ResourceType::Texture,
                    1024, 768,
                    0,
                    1, 1,
                    DX12ResourceFormat::RGBA8_UNORM,
                    DX12TextureType::RenderTarget,
                    DX12ResourceFlags::AllowRenderTarget,
                    DX12ResourceState::RenderTarget,
                    DX12HeapType::Default,
                };
            }
        }
    };
}
