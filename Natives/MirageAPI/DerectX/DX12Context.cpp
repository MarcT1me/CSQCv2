#include "pch.h"
#include "DX12Context.h"

namespace MirageAPI::DirectX
{
    void DX12Context::Initialize(bool isDebug)
    {
        if (isDebug)
        {
            ID3D12Debug* debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
            {
                debugController->EnableDebugLayer();
                s_debugController = debugController;
            }
        }

        ID3D12Device* device;
        HRESULT hr = D3D12CreateDevice(
            nullptr,
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&device)
        );
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Failed to create DX12 Device: " + hr);
        }
        s_device = device;

        HRESULT comHr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(comHr))
            DX12_CHECK(device, comHr, "COM initialization failed");
    }

    void DX12Context::Deinitialize()
    {
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
