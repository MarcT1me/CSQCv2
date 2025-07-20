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

        /* D3D12_COMMAND_QUEUE_DESC queueDesc;
         * queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
         * queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
         * queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
         * queueDesc.NodeMask = 0;
         *
         * ID3D12CommandQueue* commandQueue;
         * hr = s_device->CreateCommandQueue(
         *     &queueDesc,
         *     IID_PPV_ARGS(&commandQueue)
         * );
         * if (FAILED(hr))
         * {
         *     throw gcnew System::Exception("Failed to create D3D12 CommandQueue.");
         * }
         * s_commandQueue = commandQueue;
         */

        HRESULT comHr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (comHr != S_OK)
        {
            throw gcnew System::Exception("COM initialization failed: " + hr);
        }
    }

    void DX12Context::Deinitialize()
    {
        if (s_device)
        {
            s_device->Release();
            CoUninitialize();
            s_device = nullptr;
        }
    }
}
