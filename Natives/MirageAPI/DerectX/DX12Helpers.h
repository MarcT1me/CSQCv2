#pragma once

#define IsDXDebug DX12Device::IsDebug

namespace MirageAPI::DirectX
{
    inline void CheckHResult(HRESULT hr, const char* msg, bool checkDevice = true)
    {
        if (FAILED(hr))
        {
            if (checkDevice && hr == DXGI_ERROR_DEVICE_REMOVED)
            {
                throw gcnew HRException(
                    hr, CSFormat(
                        "{0} (hr - {1}): Device Removed (Reason - {2})",
                        gcnew System::String(msg), hr, DX12Device::GetDeviceRemovedReason()
                    )
                );
            }
            if (hr == E_INVALIDARG)
            {
                throw gcnew HRException(
                    hr, CSFormat(
                        "{0} (hr - {1}): Invalid Arguments",
                        gcnew System::String(msg), hr
                    )
                );
            }
            if (hr == E_OUTOFMEMORY)
            {
                throw gcnew HRException(
                    hr, CSFormat(
                        "{0} (hr - {1}): Out of memory",
                        gcnew System::String(msg), hr
                    )
                );
            }
            throw gcnew HRException(
                hr, CSFormat(
                    "{0} (hr - {1})",
                    gcnew System::String(msg), hr
                )
            );
        }
    }

    inline UINT GetResourceFormatSize(DX12ResourceFormat format)
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
            throw gcnew System::NotImplementedException("Unsupported resource format");
        }
    }
}
