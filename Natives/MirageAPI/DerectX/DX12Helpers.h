#pragma once

#include <d3d12.h>

#include "DX12Context.h"

namespace MirageAPI::DirectX
{
    inline void UpdateSubresources(
        ID3D12GraphicsCommandList* pCmdList,
        ID3D12Resource* pDestinationResource,
        ID3D12Resource* pIntermediate,
        UINT64 IntermediateOffset,
        unsigned int FirstSubresource,
        unsigned int NumSubresources,
        const D3D12_SUBRESOURCE_DATA* pSrcData)
    {
        // Получаем описание ресурса
        D3D12_RESOURCE_DESC desc = pDestinationResource->GetDesc();

        // Рассчитываем параметры копирования
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
        UINT64 rowSizeInBytes;
        UINT64 totalBytes;
        DX12Context::GetDevice()->GetCopyableFootprints(
            &desc,
            FirstSubresource,
            NumSubresources,
            IntermediateOffset,
            &layout,
            nullptr,
            &rowSizeInBytes,
            &totalBytes
        );

        // Копируем данные
        for (UINT i = 0; i < NumSubresources; ++i)
        {
            D3D12_TEXTURE_COPY_LOCATION dst = {
                pDestinationResource,
                D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
                FirstSubresource + i
            };

            D3D12_TEXTURE_COPY_LOCATION src = {
                pIntermediate,
                D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
                {layout.Offset + i * layout.Footprint.RowPitch * layout.Footprint.Height}
            };
            src.PlacedFootprint = layout;

            pCmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
        }
    }

    inline UINT64 GetRequiredIntermediateSize(
        ID3D12Resource* destinationResource,
        UINT firstSubresource,
        UINT numSubresources
    )
    {
        D3D12_RESOURCE_DESC desc = destinationResource->GetDesc();
        UINT64 requiredSize = 0;

        ID3D12Device* device;
        destinationResource->GetDevice(IID_PPV_ARGS(&device));
        device->GetCopyableFootprints(
            &desc, firstSubresource, numSubresources, 0,
            nullptr, nullptr, nullptr, &requiredSize
        );
        device->Release();

        return requiredSize;
    }

    inline unsigned int GetResourceFormatSize(DX12ResourceFormat format)
    {
        switch (format)
        {
        // 1 byte formats
        case DX12ResourceFormat::R8_SINT:
        case DX12ResourceFormat::R8_UINT:
        case DX12ResourceFormat::R8_SNORM:
        case DX12ResourceFormat::R8_UNORM:
            return 1;

        // 2 byte formats
        case DX12ResourceFormat::R16_UINT:
        case DX12ResourceFormat::RG8_SINT:
        case DX12ResourceFormat::RG8_UINT:
        case DX12ResourceFormat::RG8_SNORM:
        case DX12ResourceFormat::RG8_UNORM:
        case DX12ResourceFormat::BGRA4_UNORM:
        case DX12ResourceFormat::R16_FLOAT:
            return 2;

        // 4 byte formats
        case DX12ResourceFormat::R32_UINT:
        case DX12ResourceFormat::RGBA8_SINT:
        case DX12ResourceFormat::RGBA8_UINT:
        case DX12ResourceFormat::RGBA8_SNORM:
        case DX12ResourceFormat::RGBA8_UNORM:
        case DX12ResourceFormat::RGBA8_UNORM_SRGB:
        case DX12ResourceFormat::BGRA8_UNORM:
        case DX12ResourceFormat::BGRA8_UNORM_SRGB:
        case DX12ResourceFormat::R32_FLOAT:
        case DX12ResourceFormat::RG16_FLOAT:
            return 4;

        // 8 byte formats
        case DX12ResourceFormat::RGBA16_SINT:
        case DX12ResourceFormat::RGBA16_UINT:
        case DX12ResourceFormat::RGBA16_SNORM:
        case DX12ResourceFormat::RGBA16_UNORM:
        case DX12ResourceFormat::RG32_FLOAT:
        case DX12ResourceFormat::RGBA16_FLOAT:
            return 8;

        // 12 byte formats
        case DX12ResourceFormat::RGB32_SINT:
        case DX12ResourceFormat::RGB32_UINT: // Добавлено
        case DX12ResourceFormat::RGB32_FLOAT:
            return 12;

        // 16 byte formats
        case DX12ResourceFormat::RGBA32_FLOAT:
            return 16;

        // Unknown/default
        case DX12ResourceFormat::Unknown:
        default:
            throw gcnew System::NotImplementedException("Unsupported texture format");
        }
    }
}
