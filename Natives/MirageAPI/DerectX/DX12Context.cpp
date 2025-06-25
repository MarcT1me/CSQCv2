#include "pch.h"
#include "DX12Context.h"

#include <d3d12.h>
#include <dxgi1_4.h>

namespace MirageAPI::DirectX
{
    void DX12Context::Initialize()
    {
        ID3D12Device* device;
        HRESULT hr = D3D12CreateDevice(
            nullptr,
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&device)
        );
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Failed to create D3D12 Device.");
        }
        s_device = device;

        D3D12_COMMAND_QUEUE_DESC queueDesc;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;

        ID3D12CommandQueue* commandQueue;
        hr = s_device->CreateCommandQueue(
            &queueDesc,
            IID_PPV_ARGS(&commandQueue)
        );
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Failed to create D3D12 CommandQueue.");
        }
        s_commandQueue = commandQueue;
    }

    void DX12Context::Deinitialize()
    {
        if (s_commandQueue) s_commandQueue->Release();
        if (s_device) s_device->Release();
        s_commandQueue = nullptr;
        s_device = nullptr;
    }
}
