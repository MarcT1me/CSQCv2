#pragma once

namespace MirageAPI::Native::DirectX12
{
    struct  DX12DeviceConfig;
    
    class DX12Device
    {
        static DX12Device* _dxDevice;
        static bool _isDebug;

        ID3D12Device* _device = nullptr;
        ID3D12Debug* _debug = nullptr;
        IDXGIFactory4* _factory = nullptr;

    public:
        ID3D12Device* GetNativeDevice() const { return _device; }
        ID3D12Debug* GetNativeDebug() const { return _debug; }
        IDXGIFactory4* GetNativeFactory() const { return _factory; }

        // static
        static DX12Device* GetDXDevice() { return _dxDevice; }
        static bool IsDebug() { return _isDebug; }
        static bool IsInitialized() { return _dxDevice != nullptr; }
        static HRESULT GetDeviceRemovedReason();
        
        DX12Device(const DX12DeviceConfig& config);
        ~DX12Device();
    };
}
