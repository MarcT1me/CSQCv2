#include "pch.h"
#include "DX12Texture.h"

#include "DX12UploadBuffer.h"
#include "..\DX12Helpers.h"
#include "..\CommandList\DX12CommandList.h"

namespace MirageAPI::DirectX
{
    DX12ResourceConfig GetTextureConfig(
        unsigned int width, unsigned int height,
        DX12ResourceFormat format,
        DX12TextureType type,
        unsigned int mipLevels,
        DX12ResourceFlags flags
    )
    {
        DX12ResourceConfig config;
        config.Type = DX12ResourceType::Texture;
        config.Width = width;
        config.Height = height;
        config.Format = format;
        config.TextureType = type;
        config.MipLevels = mipLevels;
        config.Flags = flags;
        config.Stride = GetResourceFormatSize(format);
        config.Depth = 1;
        config.HeapType = DX12HeapType::Default;
        config.InitialState = DX12ResourceState::Common;
        return config;
    }

    DX12Texture::DX12Texture(
        unsigned int width, unsigned int height,
        DX12ResourceFormat format,
        DX12TextureType type,
        unsigned int mipLevels,
        DX12ResourceFlags flags
    ) : DX12Buffer(GetTextureConfig(width, height, format, type, mipLevels, flags)),
        m_type(type),
        m_width(width),
        m_height(height),
        m_mipLevels(mipLevels),
        m_generateMipmaps(mipLevels > 1)
    {
    }

    void DX12Texture::UploadData(std::byte data[], int width, int height)
    {
        if (!m_nativeResource || !data || width <= 0 || height <= 0) return;

        const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_nativeResource, 0, 1);

        DX12ResourceConfig uploadConfig;
        uploadConfig.Type = DX12ResourceType::UploadBuffer;
        uploadConfig.Width = uploadBufferSize;
        uploadConfig.HeapType = DX12HeapType::Upload;
        uploadConfig.InitialState = DX12ResourceState::GenericRead;

        auto uploadBuffer = gcnew DX12UploadBuffer(uploadConfig);

        void* pData = uploadBuffer->Map();
        memcpy(pData, data, uploadBufferSize);
        uploadBuffer->Unmap();

        DX12CommandList^ commandList = gcnew DX12CommandList(DX12CommandListType::Direct);
        commandList->Reset();

        this->TransitionState(commandList, DX12ResourceState::CopyDest);

        D3D12_SUBRESOURCE_DATA subresourceData;
        subresourceData.pData = &data[0];
        subresourceData.RowPitch = static_cast<LONG_PTR>(width * GetResourceFormatSize(m_format));
        subresourceData.SlicePitch = subresourceData.RowPitch * height;

        UpdateSubresources(
            commandList->NativeCommandList,
            m_nativeResource,
            uploadBuffer->NativeResource,
            0, 0, 1,
            &subresourceData
        );

        // Возвращаем в исходное состояние
        this->TransitionState(commandList, DX12ResourceState::Common);

        // Выполняем команды
        commandList->Close();
        commandList->Execute();
        commandList->WaitForCompletion();

        delete commandList;
        delete uploadBuffer;

        if (m_generateMipmaps)
        {
            GenerateMipmaps(commandList);
        }
    }

    void DX12Texture::GenerateMipmaps(DX12CommandList^ commandList)
    {
        // Реализация генерации мипмапов будет добавлена позже
        // Это сложная операция, требующая вычислительных шейдеров
        // или последовательного рендеринга в уменьшающиеся текстуры
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC DX12Texture::CreateSRVDesc()
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = static_cast<DXGI_FORMAT>(m_format);
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = m_elementCount;
        srvDesc.Buffer.StructureByteStride = m_stride;
        srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
        return srvDesc;
    }
}
