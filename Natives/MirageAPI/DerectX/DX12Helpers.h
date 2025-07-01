#pragma once

#include <d3d12.h>

namespace MirageAPI::DirectX
{
    inline void UpdateSubresources(
        ID3D12GraphicsCommandList* pCmdList,
        ID3D12Resource* pDestinationResource,
        ID3D12Resource* pIntermediate,
        UINT64 IntermediateOffset,
        UINT FirstSubresource,
        UINT NumSubresources,
        const D3D12_SUBRESOURCE_DATA* pSrcData
    )
    {
        // Упрощенная реализация для одного субресурса
        if (NumSubresources == 1)
        {
            D3D12_RESOURCE_DESC desc = pDestinationResource->GetDesc();
            D3D12_TEXTURE_COPY_LOCATION dst = {
                pDestinationResource,
                D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
                FirstSubresource
            };
            D3D12_TEXTURE_COPY_LOCATION src = {
                pIntermediate,
                D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
                {
                    IntermediateOffset, {
                        desc.Format,
                        static_cast<UINT>(desc.Width),
                        static_cast<UINT>(desc.Height),
                        static_cast<UINT>(desc.DepthOrArraySize),
                        static_cast<UINT>(pSrcData->RowPitch)
                    }
                }
            };
            pCmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
        }
        else
        {
            // Для нескольких субресурсов потребуется более сложная реализация
            throw gcnew System::NotImplementedException("Multi-subresource updates not implemented");
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

    inline UINT GetTextureFormatSize(DX12ResourceFormat format)
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
            return 1;
        }
    }
}
