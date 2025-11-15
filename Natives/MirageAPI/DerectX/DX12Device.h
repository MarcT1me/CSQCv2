#pragma once

namespace MirageAPI::OpenXR
{
    ref class XRInstance;
}

namespace MirageAPI::DirectX
{
    public ref class DX12DeviceRequirements
    {
        LUID* _adapterLuid;
        D3D_FEATURE_LEVEL _featureLevel;

    public:
        property LUID* AdapterLuid
        {
            LUID* get() { return _adapterLuid; }
        }

        property D3D_FEATURE_LEVEL FeatureLevel
        {
            D3D_FEATURE_LEVEL get() { return _featureLevel; }
        }

        DX12DeviceRequirements(LUID* adapterLuid, D3D_FEATURE_LEVEL featureLevel)
            : _adapterLuid(adapterLuid), _featureLevel(featureLevel)
        {
        }
    };

    public ref class DX12Device
    {
        static ID3D12Device* s_device;
        static ID3D12Debug* s_debugController;
        static IDXGIFactory4* s_factory;
        static bool s_isDebug;

    public:
        static property bool IsDebug
        {
            bool get() { return s_isDebug; }
        }

        static void Initialize(DX12DeviceInitFlags flags, DX12DeviceRequirements^ requirements);
        static void Deinitialize();

        static HRESULT GetDeviceRemovedReason()
        {
            if (!s_device)
                throw gcnew DXException("Device not initialized - cant get device remove reason");
            return s_device->GetDeviceRemovedReason();
        }

        static ID3D12Device* GetNativeDevice(bool autoThrow)
        {
            if (s_device == nullptr && autoThrow)
                throw gcnew DXException("DX12 device not initialized");
            return s_device;
        }

        static IDXGIFactory4* GetNativeFactory(bool autoThrow)
        {
            if (s_factory == nullptr && autoThrow)
                throw gcnew DXException("DX12 factory not initialized");
            return s_factory;
        }

        static IntPtr^ GetNativeDeviceIntPtr()
        {
            return gcnew IntPtr(GetNativeDevice(true));
        }
    };
}
