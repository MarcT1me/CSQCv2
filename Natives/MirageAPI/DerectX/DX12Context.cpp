#include "pch.h"
#include "DX12Context.h"

#include <dxgi1_6.h>

namespace MirageAPI::DirectX
{
    inline void GetHardwareAdapter(
        IDXGIFactory1* factory,
        IDXGIAdapter1** outAdapter,
        bool isHighPerformance = false
    )
    {
        IDXGIAdapter1* adapter;

        IDXGIFactory6* factory6;
        if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
        {
            for (UINT adapterIndex = 0;
                 SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                     adapterIndex,
                     isHighPerformance == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE :
                     DXGI_GPU_PREFERENCE_UNSPECIFIED,
                     IID_PPV_ARGS(&adapter)
                 ));
                 ++adapterIndex)
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    continue;
                }

                if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        if (adapter == nullptr)
        {
            for (
                UINT adapterIndex = 0;
                SUCCEEDED(factory->EnumAdapters1(adapterIndex, &adapter));
                ++adapterIndex
            )
            {
                DXGI_ADAPTER_DESC1 desc;
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    continue;
                }

                if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        *outAdapter = adapter;
    }

    void DX12Context::Initialize(DX12ContextInitFlags flags)
    {
        bool isDebug = flags.HasFlag(DX12ContextInitFlags::Debug);

        // create debug layer
        if (isDebug)
        {
            ID3D12Debug* debugController;
            CheckHResult(
                D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)),
                "create debug controller failed"
            );
            debugController->EnableDebugLayer();
            s_debugController = debugController;
        }

        // create factory
        IDXGIFactory4* factory = nullptr;
        UINT factoryFlags = isDebug ? DXGI_CREATE_FACTORY_DEBUG : 0;
        CheckHResult(
            CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory)),
            "Create Factory failed", false
        );
        s_factory = factory;

        // create DX adapter and device
        HRESULT hr;
        ID3D12Device* device;
        if (flags.HasFlag(DX12ContextInitFlags::UseAdapter))
        {
            if (flags.HasFlag(DX12ContextInitFlags::UseWarpAdapter))
            {
                IDXGIAdapter* warpAdapter;
                CheckHResult(
                    factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)),
                    "Failed to enumerate warp adapter", false
                );

                hr = D3D12CreateDevice(
                    warpAdapter,
                    D3D_FEATURE_LEVEL_11_0,
                    IID_PPV_ARGS(&device)
                );
            }
            else
            {
                IDXGIAdapter1* hardwareAdapter;
                GetHardwareAdapter(
                    factory, &hardwareAdapter, flags.HasFlag(DX12ContextInitFlags::UseHighPerformanceAdapter)
                );

                hr = D3D12CreateDevice(
                    hardwareAdapter,
                    D3D_FEATURE_LEVEL_11_0,
                    IID_PPV_ARGS(&device)
                );
            }
        }
        else
        {
            hr = D3D12CreateDevice(
                nullptr,
                D3D_FEATURE_LEVEL_11_0,
                IID_PPV_ARGS(&device)
            );
        }
        CheckHResult(hr, "Failed to create device", false);
        s_device = device;

        // init COM
        CheckHResult(
            CoInitializeEx(nullptr, COINIT_MULTITHREADED),
            "COM initialization failed"
        );
    }

    void DX12Context::Deinitialize()
    {
        if (s_factory)
        {
            s_factory->Release();
            s_factory = nullptr;
        }
        if (s_device)
        {
            CoUninitialize();
            s_device->Release();
            s_debugController->Release();
            s_debugController = nullptr;
            s_device = nullptr;
        }
    }
}
