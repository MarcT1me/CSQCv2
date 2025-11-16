// ReSharper disable CppClangTidyClangDiagnosticLanguageExtensionToken
#include "pch.h"
#include "DX12Device.h"

#include "DX12DeviceConfig.h"
#include <dxgi1_6.h>

namespace MirageAPI::Native::DirectX12
{
    DX12Device* DX12Device::_dxDevice = nullptr;
    bool DX12Device::_isDebug = false;
    
    static auto GetHardwareAdapter(
        IDXGIFactory1* factory,
        const DX12DeviceConfig& config,
        IDXGIAdapter1** outAdapter
    ) -> void
    {
        *outAdapter = nullptr;

        if (config.AdapterType & DX12DeviceAdapterType::XR && config.deviceRequirements != nullptr)
        {
            LUID adapterLuid = config.deviceRequirements->GetAdapterLuid();

            for (UINT adapterIndex = 0; ; ++adapterIndex)
            {
                IDXGIAdapter1* adapter = nullptr;
                if (FAILED(factory->EnumAdapters1(adapterIndex, &adapter)))
                    break;

                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                // skip software
                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    adapter->Release();
                    continue;
                }

                if (desc.AdapterLuid.LowPart == adapterLuid.LowPart && desc.AdapterLuid.HighPart == adapterLuid.HighPart)
                {
                    // check adapter for feature level
                    D3D_FEATURE_LEVEL featureLevel = config.deviceRequirements->GetFeatureLevel();
                    if (SUCCEEDED(D3D12CreateDevice(adapter, featureLevel, _uuidof(ID3D12Device), nullptr)))
                    {
                        *outAdapter = adapter;
                        return;
                    }
                }
                adapter->Release();
            }
        }
        else
        {
            bool isHighPerformance = static_cast<bool>(config.AdapterType & DX12DeviceAdapterType::HighPerformance);

            // try to use IDXGIFactory6
            IDXGIFactory6* factory6 = nullptr;
            if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
            {
                for (UINT adapterIndex = 0;
                     SUCCEEDED(
                         factory6->EnumAdapterByGpuPreference(
                             adapterIndex,
                             isHighPerformance ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                             IID_PPV_ARGS(outAdapter))
                     );
                     ++adapterIndex)
                {
                    DXGI_ADAPTER_DESC1 desc;
                    (*outAdapter)->GetDesc1(&desc);

                    if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                    {
                        (*outAdapter)->Release();
                        continue;
                    }

                    // check adapter for feature level
                    D3D_FEATURE_LEVEL featureLevel = config.GetFeatureLevel();
                    if (SUCCEEDED(D3D12CreateDevice(*outAdapter, featureLevel, _uuidof(ID3D12Device), nullptr)))
                    {
                        factory6->Release();
                        return;
                    }
                    (*outAdapter)->Release();
                }
                factory6->Release();
            }

            // Fallback
            for (UINT adapterIndex = 0; SUCCEEDED(factory->EnumAdapters1(adapterIndex, outAdapter)); ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                (*outAdapter)->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    (*outAdapter)->Release();
                    continue;
                }

                if (SUCCEEDED(D3D12CreateDevice(*outAdapter, config.GetFeatureLevel(), _uuidof(ID3D12Device), nullptr)))
                {
                    return;
                }
                (*outAdapter)->Release();
            }
        }
    }

    DX12Device::DX12Device(const DX12DeviceConfig& config)
    {
        if (IsInitialized()) return;

        _isDebug = config.IsDebug;

        if (_isDebug)
        {
            DX12ResultChecker::Check(
                D3D12GetDebugInterface(IID_PPV_ARGS(&_debug)),
                "Create Device Debug Controller"
            );
            _debug->EnableDebugLayer();
        }

        DX12ResultChecker::Check(
            CreateDXGIFactory2(_isDebug ? DXGI_CREATE_FACTORY_DEBUG : 0, IID_PPV_ARGS(&_factory)),
            "Create Device Factory"
        );

        IUnknown* deviceAdapter = nullptr;
        if (config.AdapterType & DX12DeviceAdapterType::Simple)
        {
            if (config.AdapterType & DX12DeviceAdapterType::Warp)
            {
                DX12ResultChecker::Check(
                    _factory->EnumWarpAdapter(IID_PPV_ARGS(&deviceAdapter)),
                    "Enumerate Warp Adapter"
                );
            }
            else
            {
                GetHardwareAdapter(_factory, config, reinterpret_cast<IDXGIAdapter1**>(&deviceAdapter));
            }
        }
        DX12ResultChecker::Check(
            D3D12CreateDevice(deviceAdapter, config.GetFeatureLevel(), IID_PPV_ARGS(&_device)),
            "create DX12Device"
        );

        DX12ResultChecker::Check(
            CoInitializeEx(nullptr, COINIT_MULTITHREADED),
            "COM Initialization"
        );

        _dxDevice = this;
    }

    DX12Device::~DX12Device()
    {
        if (IsInitialized()) CoUninitialize();
        SimpleRelease(_device)
        SimpleRelease(_factory)
        SimpleRelease(_debug)
        _dxDevice = nullptr;
    }

    HRESULT DX12Device::GetDeviceRemovedReason()
    {
        return _dxDevice->GetNativeDevice()->GetDeviceRemovedReason();
    }
}
