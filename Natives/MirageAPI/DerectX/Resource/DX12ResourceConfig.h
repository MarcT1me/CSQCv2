#pragma once

#include "enums.h"

namespace MirageAPI::DirectX::Resource
{
    public value struct DX12ResourceConfig
    {
        DX12ResourceType Type;

        UINT Width;
        UINT Height;
        UINT Stride;

        DX12ResourceFormat Format;
        DX12ResourceFlags Flags;

        UINT Depth;
        UINT MipLevels;
        DX12TextureType TextureType;

        DX12ViewDimension Dimension;
        DX12ResourceState InitialState;
        DX12HeapType HeapType;

        static DX12ResourceConfig BufferConfig(
            DX12ResourceType type,
            UINT size,
            DX12ResourceFlags flags
        )
        {
            return {
                .Type = type,
                .Width = size,
                .Height = 1,
                .Stride = 1,
                .Format = DX12ResourceFormat::Unknown,
                .Flags = flags,
                .Dimension = DX12ViewDimension::Buffer,
                .HeapType = DX12HeapType::Upload
            };
        }

        static DX12ResourceConfig VertexBufferConfig(
            UINT count,
            UINT stride,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::VertexBuffer, count, flags);
            config.Stride = stride;

            config.InitialState = DX12ResourceState::GenericRead;
            return config;
        }

        static DX12ResourceConfig IndexBufferConfig(
            UINT count,
            DX12ResourceFormat format,
            DX12ResourceFlags flags
        )
        {
            CheckMissmatch(format, DX12ResourceFormat::R16_UINT)
                CheckMissmatch(format, DX12ResourceFormat::R32_UINT)
                {
                    throw gcnew System::ArgumentException("Invalid index buffer format");
                }

            DX12ResourceConfig config = BufferConfig(DX12ResourceType::IndexBuffer, count, flags);
            config.Stride = GetResourceFormatSize(format);
            config.Format = format;

            config.InitialState = DX12ResourceState::IndexBuffer;
            return config;
        }

        static DX12ResourceConfig ConstantBufferConfig(
            UINT size,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::ConstantBuffer, size, flags);

            config.InitialState = DX12ResourceState::VertexAndConstantBuffer;
            return config;
        }

        static DX12ResourceConfig UploadBufferConfig(
            UINT size,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::UploadBuffer, size, flags);

            config.InitialState = DX12ResourceState::GenericRead;
            return config;
        }

        static DX12ResourceConfig StructuredBufferConfig(
            UINT elementCount,
            UINT stride,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::StructuredBuffer, elementCount, flags);
            config.Stride = stride;

            config.InitialState = DX12ResourceState::Common;
            return config;
        }

        static DX12ResourceConfig TextureConfig(
            UINT width,
            UINT height,
            DX12ResourceFormat format,
            DX12ResourceFlags flags,
            UINT depth,
            UINT mipLevels,
            DX12TextureType type,
            DX12ViewDimension dimension
        )
        {
            return {
                .Type = DX12ResourceType::Texture,

                .Width = width,
                .Height = height,
                .Stride = GetResourceFormatSize(format),

                .Format = format,
                .Flags = flags,

                .Depth = depth,
                .MipLevels = mipLevels,
                .TextureType = type,

                .Dimension = dimension,
                .InitialState = DX12ResourceState::Common,
                .HeapType = DX12HeapType::Default
            };
        }

        static DX12ResourceConfig RenderTargetConfig(
            UINT width,
            UINT height,
            DX12ResourceFormat format,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig config = TextureConfig(
                width,
                height,
                format,
                DX12ResourceFlags::AllowRenderTarget | flags,
                1,
                1,
                DX12TextureType::Texture2D,
                DX12ViewDimension::Texture2D
            );
            config.Type = DX12ResourceType::RenderTarget;
            config.InitialState = DX12ResourceState::Present;
            return config;
        }
    };
}
