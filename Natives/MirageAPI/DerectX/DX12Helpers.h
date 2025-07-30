#pragma once

#define SimpleDelete(resource) if ((resource)) { delete (resource); (resource) = nullptr; }
#define CheckNull(resource, exc) if ((resource)) throw (exc)

#define DXSimpleRelease(resource) if ((resource) && (resource)->Release() == 0) (resource) = nullptr;
#define DXSimpleDeleteArr(resource) if ((resource)) { delete[] (resource); (resource) = nullptr; }

namespace MirageAPI::DirectX
{
    inline void CheckHResult(HRESULT hr, const char* msg, bool checkDevice = true)
    {
        if (FAILED(hr))
        {
            if (checkDevice && hr == DXGI_ERROR_DEVICE_REMOVED)
                throw gcnew HRException(
                    hr, gcnew System::String(msg) + gcnew System::String(
                        ": Device removed (" + DX12Device::GetDeviceRemovedReason() + ")"
                    ));
            throw gcnew HRException(
                hr, gcnew System::String(msg) + gcnew System::String(
                    ": " + hr
                ));
        }
    }

    inline bool IsDebug() { return DX12Device::IsDebug; }

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
            throw gcnew System::NotImplementedException("Unsupported texture format");
        }
    }
}
