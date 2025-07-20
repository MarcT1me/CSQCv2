#pragma once
#include "../DX12Helpers.h"

namespace MirageAPI::DirectX
{
    public value struct DX12ResourceConfig
    {
        DX12ResourceType Type;

        unsigned int Width;
        unsigned int Height;
        unsigned int Stride;

        unsigned int Depth;
        unsigned int MipLevels;

        DX12ResourceFormat Format;
        DX12TextureType TextureType;

        DX12ResourceFlags Flags;
        DX12ResourceState InitialState;
        DX12HeapType HeapType;

        static DX12ResourceConfig BufferConfig(DX12ResourceType type)
        {
            DX12ResourceConfig config;
            config.Type = type;
            config.Height = 1;
            config.Format = DX12ResourceFormat::Unknown;
            config.TextureType = DX12TextureType::None;
            config.Flags = DX12ResourceFlags::None;
            return config;
        }

        static DX12ResourceConfig VertexBufferConfig(
            unsigned int elementCount,
            unsigned int stride
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::VertexBuffer);
            config.Width = elementCount;
            config.Stride = stride;

            config.InitialState = DX12ResourceState::VertexAndConstantBuffer;
            config.HeapType = DX12HeapType::Upload;
            return config;
        }

        static DX12ResourceConfig IndexBufferConfig(
            unsigned int elementCount,
            DX12ResourceFormat format
        )
        {
            if (format != DX12ResourceFormat::R16_UINT &&
                format != DX12ResourceFormat::R32_UINT)
            {
                throw gcnew System::ArgumentException("Invalid index buffer format");
            }

            DX12ResourceConfig config = BufferConfig(DX12ResourceType::IndexBuffer);
            config.Width = elementCount;
            config.Stride = GetResourceFormatSize(format);
            config.Format = format;

            config.InitialState = DX12ResourceState::IndexBuffer;
            config.HeapType = DX12HeapType::Upload;
            return config;
        }

        static DX12ResourceConfig UploadBufferConfig(
            unsigned int size
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::UploadBuffer);
            config.Width = size;
            config.Stride = 1;

            config.InitialState = DX12ResourceState::GenericRead;
            config.HeapType = DX12HeapType::Upload;
            return config;
        }

        static DX12ResourceConfig ConstantBufferConfig()
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::ConstantBuffer);
            config.Width = 256;

            config.InitialState = DX12ResourceState::GenericRead;
            config.HeapType = DX12HeapType::Upload;
            return config;
        }

        static DX12ResourceConfig StructuredBufferConfig(
            unsigned int elementCount,
            unsigned int stride,
            DX12ResourceFlags flags,
            DX12HeapType heapType
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::StructuredBuffer);
            config.Width = elementCount;
            config.Stride = stride;

            config.Flags = flags;
            config.InitialState = DX12ResourceState::Common;
            config.HeapType = heapType;
            return config;
        }

        static DX12ResourceConfig TextureConfig(
            unsigned int width,
            unsigned int height,
            DX12ResourceFormat format,
            unsigned int depth,
            unsigned int mipLevels,
            DX12TextureType type,
            DX12ResourceFlags flags
        )
        {
            return {
                DX12ResourceType::Texture,
                width,
                height,
                GetResourceFormatSize(format),
                depth,
                mipLevels,
                format,
                type,
                flags,
                DX12ResourceState::Common,
                DX12HeapType::Default
            };
        }

        static DX12ResourceConfig RenderTargetConfig(
            unsigned int width,
            unsigned int height,
            DX12ResourceFormat format
        )
        {
            DX12ResourceConfig config = TextureConfig(
                width,
                height,
                format,
                1,
                1,
                DX12TextureType::RenderTarget,
                DX12ResourceFlags::AllowRenderTarget
            );
            config.Type = DX12ResourceType::FrameBuffer;
            config.InitialState = DX12ResourceState::Present;
            config.HeapType = DX12HeapType::Default;
            return config;
        }
    };
}
