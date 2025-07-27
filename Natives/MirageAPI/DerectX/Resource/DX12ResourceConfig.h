#pragma once

namespace MirageAPI::DirectX::Resource
{
    public value struct DX12ResourceConfig
    {
        DX12ResourceType Type;

        UINT Width;
        UINT Height;
        UINT Stride;

        UINT Depth;
        UINT MipLevels;

        DX12ResourceFormat Format;
        DX12TextureType TextureType;

        DX12ResourceFlags Flags;
        DX12ResourceState InitialState;
        DX12HeapType HeapType;

        static DX12ResourceConfig BufferConfig(DX12ResourceType type, UINT size)
        {
            DX12ResourceConfig config;
            config.Type = type;

            config.Width = size;
            config.Height = 1;
            config.Stride = 1;

            config.Format = DX12ResourceFormat::Unknown;
            config.TextureType = DX12TextureType::None;
            config.Flags = DX12ResourceFlags::None;
            return config;
        }

        static DX12ResourceConfig VertexBufferConfig(
            UINT elementCount,
            UINT stride
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::VertexBuffer, elementCount);
            config.Stride = stride;

            config.InitialState = DX12ResourceState::VertexAndConstantBuffer;
            config.HeapType = DX12HeapType::Upload;
            return config;
        }

        static DX12ResourceConfig IndexBufferConfig(
            UINT elementCount,
            DX12ResourceFormat format
        )
        {
            if (format != DX12ResourceFormat::R16_UINT &&
                format != DX12ResourceFormat::R32_UINT)
            {
                throw gcnew System::ArgumentException("Invalid index buffer format");
            }

            DX12ResourceConfig config = BufferConfig(DX12ResourceType::IndexBuffer, elementCount);
            config.Stride = GetResourceFormatSize(format);
            config.Format = format;

            config.InitialState = DX12ResourceState::IndexBuffer;
            config.HeapType = DX12HeapType::Upload;
            return config;
        }

        static DX12ResourceConfig ConstantBufferConfig()
        {
            return UploadBufferConfig(256);
        }

        static DX12ResourceConfig UploadBufferConfig(
            UINT size
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::UploadBuffer, size);

            config.InitialState = DX12ResourceState::GenericRead;
            config.HeapType = DX12HeapType::Upload;
            return config;
        }

        static DX12ResourceConfig StructuredBufferConfig(
            UINT elementCount,
            UINT stride,
            DX12ResourceFlags flags,
            DX12HeapType heapType
        )
        {
            DX12ResourceConfig config = BufferConfig(DX12ResourceType::StructuredBuffer, elementCount);
            config.Stride = stride;

            config.Flags = flags;
            config.InitialState = DX12ResourceState::Common;
            config.HeapType = heapType;
            return config;
        }

        static DX12ResourceConfig TextureConfig(
            UINT width,
            UINT height,
            DX12ResourceFormat format,
            UINT depth,
            UINT mipLevels,
            DX12TextureType type,
            DX12ResourceFlags flags
        )
        {
            return {
                .Type = DX12ResourceType::Texture,
                
                .Width = width,
                .Height = height,
                .Stride = GetResourceFormatSize(format),
                
                .Depth = depth,
                .MipLevels = mipLevels,
                
                .Format = format,
                .TextureType = type,
                
                .Flags = flags,
                .InitialState = DX12ResourceState::Common,
                .HeapType = DX12HeapType::Default
            };
        }

        static DX12ResourceConfig RenderTargetConfig(
            UINT width,
            UINT height,
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
