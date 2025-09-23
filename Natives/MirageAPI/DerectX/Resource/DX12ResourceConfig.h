#pragma once

#include "../DX12ObjectData.h"

namespace MirageAPI::DirectX::Resource
{
    public ref class DX12ResourceConfig : public DX12ObjectData
    {
        DX12ResourceConfig(
            QIdentifier^ identifier,
            DX12ResourceType Type,
            Shader::DX12ShaderVisibility shaderVisibility,
            UINT Width,
            UINT Height,
            UINT Stride,
            DX12ResourceFormat Format,
            DX12ResourceFlags Flags,
            UINT Depth,
            UINT MipLevels,
            DX12TextureType TextureType,
            UINT SampleCount,
            UINT SampleQuality,
            DX12ViewDimension Dimension,
            DX12ResourceState InitialState,
            Descriptors::DX12HeapType HeapType
        ) : DX12ObjectData(identifier),
            Type(Type),
            ShaderVisibility(shaderVisibility),
            Width(Width),
            Height(Height),
            Stride(Stride),
            Format(Format),
            Flags(Flags),
            Depth(Depth),
            MipLevels(MipLevels),
            TextureType(TextureType),
            SampleCount(SampleCount),
            SampleQuality(SampleQuality),
            Dimension(Dimension),
            InitialState(InitialState),
            HeapType(HeapType)
        {
        }

    public:
        DX12ResourceType Type;
        Shader::DX12ShaderVisibility ShaderVisibility;

        UINT Width;
        UINT Height;
        UINT Stride;

        DX12ResourceFormat Format;
        DX12ResourceFlags Flags;

        UINT Depth;
        UINT MipLevels;
        DX12TextureType TextureType;
        UINT SampleCount;
        UINT SampleQuality;

        DX12ViewDimension Dimension;
        DX12ResourceState InitialState;
        Descriptors::DX12HeapType HeapType;

        static DX12ResourceConfig^ BufferConfig(
            QIdentifier^ identifier,
            DX12ResourceType type,
            UINT size,
            DX12ResourceFlags flags
        )
        {
            return gcnew DX12ResourceConfig(
                identifier,
                type,
                Shader::DX12ShaderVisibility::All,
                size,
                1,
                1,
                DX12ResourceFormat::Unknown,
                flags,
                0, 0,
                DX12TextureType::Unknown,
                0, 0,
                DX12ViewDimension::Buffer,
                DX12ResourceState::Common,
                Descriptors::DX12HeapType::Upload
            );
        }

        static DX12ResourceConfig^ VertexBufferConfig(
            QIdentifier^ identifier,
            UINT count,
            UINT stride,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig^ config = BufferConfig(identifier, DX12ResourceType::VertexBuffer, count, flags);
            config->Stride = stride;

            config->InitialState = DX12ResourceState::GenericRead;
            return config;
        }

        static DX12ResourceConfig^ IndexBufferConfig(
            QIdentifier^ identifier,
            UINT count,
            DX12ResourceFormat format,
            DX12ResourceFlags flags
        )
        {
            CheckMissmatch(format, DX12ResourceFormat::R16_UINT)
                CheckMissmatch(format, DX12ResourceFormat::R32_UINT)
                {
                    throw gcnew DXException("Invalid index buffer format");
                }

            DX12ResourceConfig^ config = BufferConfig(identifier, DX12ResourceType::IndexBuffer, count, flags);
            config->Stride = GetResourceFormatSize(format);
            config->Format = format;

            config->InitialState = DX12ResourceState::GenericRead;
            return config;
        }

        static DX12ResourceConfig^ ConstantBufferConfig(
            QIdentifier^ identifier,
            Shader::DX12ShaderVisibility visibility,
            UINT size,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig^ config = BufferConfig(identifier, DX12ResourceType::ConstantBuffer, size, flags);
            config->ShaderVisibility = visibility;

            config->InitialState = DX12ResourceState::GenericRead;
            return config;
        }

        static DX12ResourceConfig^ UploadBufferConfig(
            UINT size,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig^ config = BufferConfig(gcnew String("UploadBuffer" + size), DX12ResourceType::UploadBuffer, size, flags);

            config->InitialState = DX12ResourceState::GenericRead;
            return config;
        }

        static DX12ResourceConfig^ StructuredBufferConfig(
            QIdentifier^ identifier,
            Shader::DX12ShaderVisibility visibility,
            UINT elementCount,
            UINT stride,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig^ config = BufferConfig(identifier, DX12ResourceType::StructuredBuffer, elementCount, flags);
            config->ShaderVisibility = visibility;
            config->Stride = stride;

            config->InitialState = DX12ResourceState::Common;
            return config;
        }

        static DX12ResourceConfig^ TextureConfig(
            QIdentifier^ identifier,
            Shader::DX12ShaderVisibility visibility,
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
            return gcnew DX12ResourceConfig(
                identifier,
                DX12ResourceType::Texture,
                visibility,
                width,
                height,
                GetResourceFormatSize(format),
                format,
                flags,
                depth, mipLevels,
                type,
                1, 0,
                dimension,
                DX12ResourceState::CopyDest,
                Descriptors::DX12HeapType::Default
            );
        }

        static DX12ResourceConfig^ RenderTargetConfig(
            QIdentifier^ identifier,
            UINT width,
            UINT height,
            DX12ResourceFormat format,
            DX12ResourceFlags flags
        )
        {
            DX12ResourceConfig^ config = TextureConfig(
                identifier,
                Shader::DX12ShaderVisibility::All,
                width,
                height,
                format,
                flags,
                1,
                1,
                DX12TextureType::Texture2D,
                DX12ViewDimension::Texture2D
            );
            config->Type = DX12ResourceType::RenderTarget;
            config->InitialState = DX12ResourceState::Present;
            config->SampleCount = 0;
            return config;
        }
    };
}
