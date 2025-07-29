#pragma once

#include "DX12Context.h"

namespace MirageAPI::DirectX
{
    public ref class DXException : public System::Exception
    {
    public:
        DXException(System::String^ msg) : Exception(gcnew System::String(msg))
        {
        }
    };
    
    public ref class HRException : public DXException
    {
        HRESULT m_hr;

    public:
        HRException(HRESULT hr, System::String^ msg) : DXException(gcnew System::String(msg)), m_hr(hr)
        {
        }

        HRESULT HR() { return m_hr; }
    };
    
    inline ID3D12Device* GetContextDevice(bool autoThrow = true)
    {
        if (DX12Context::s_device == nullptr && autoThrow)
            throw gcnew DXException("DX12 device not initialized");
        return DX12Context::s_device;
    }
    
    inline IDXGIFactory4* GetContextFactory(bool autoThrow = true)
    {
        if (DX12Context::s_factory == nullptr && autoThrow)
            throw gcnew DXException("DX12 factory not initialized");
        return DX12Context::s_factory;
    }

    inline void CheckHResult(HRESULT hr, const char* msg, bool checkDevice = true)
    {
        if (FAILED(hr))
        {
            if (checkDevice && hr == DXGI_ERROR_DEVICE_REMOVED)
                throw gcnew HRException(
                    hr, gcnew System::String(msg) + gcnew System::String(
                        ": Device removed (" + GetContextDevice()->GetDeviceRemovedReason() + ")"
                    ));
            throw gcnew HRException(
                hr, gcnew System::String(msg) + gcnew System::String(
                    ": " + hr
                ));
        }
    }

    inline void UpdateSubresources(
        ID3D12GraphicsCommandList* pCmdList,
        ID3D12Resource* pDestinationResource,
        ID3D12Resource* pIntermediate,
        UINT64 IntermediateOffset,
        UINT FirstSubresource,
        UINT NumSubresources
    )
    {
        // Получаем описание ресурса
        D3D12_RESOURCE_DESC desc = pDestinationResource->GetDesc();

        // Рассчитываем параметры копирования
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
        UINT64 rowSizeInBytes;
        UINT64 totalBytes;
        GetContextDevice()->GetCopyableFootprints(
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

    inline UINT GetResourceFormatSize(
        DX12ResourceFormat format
    )
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
