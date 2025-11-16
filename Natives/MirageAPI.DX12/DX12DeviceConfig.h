#pragma once

namespace MirageAPI::Native::DirectX12
{
    struct DX12DeviceRequirements
    {
    private:
        LUID _adapterLuid;
        D3D_FEATURE_LEVEL _featureLevel;

    public:
        LUID GetAdapterLuid() const { return _adapterLuid; }
        D3D_FEATURE_LEVEL GetFeatureLevel() const { return _featureLevel; }

        DX12DeviceRequirements(LUID adapterLuid, D3D_FEATURE_LEVEL featureLevel)
            : _adapterLuid(adapterLuid), _featureLevel(featureLevel)
        {
        }
    };

    enum class DX12DeviceAdapterType : uint8_t
    {
        None = 0,
        Simple = 1,
        Warp = Simple | 2,
        HighPerformance = Simple | 4,
        XR = HighPerformance | 8,
    };

    inline bool operator&(DX12DeviceAdapterType lhs, DX12DeviceAdapterType rhs)
    {
        return static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs);
    }

    struct DX12DeviceConfig
    {
        bool IsDebug;
        DX12DeviceAdapterType AdapterType = DX12DeviceAdapterType::None;
        DX12DeviceRequirements* deviceRequirements = nullptr;

        D3D_FEATURE_LEVEL GetFeatureLevel() const
        {
            return AdapterType & DX12DeviceAdapterType::XR && deviceRequirements != nullptr
                       ? deviceRequirements->GetFeatureLevel()
                       : D3D_FEATURE_LEVEL_11_0;
        }
    };
}
