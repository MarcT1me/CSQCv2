#pragma once

namespace MirageAPI::DirectX
{
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

        static void Initialize(DX12DeviceInitFlags flags);
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
    };
}
