#include "pch.h"
#include "DX12Context.h"

#include <algorithm>

#include "DX12DescriptorHeapManager.h"

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
        if (hr == DXGI_ERROR_UNSUPPORTED)
        {
            throw gcnew System::Exception("DirectX 12 не поддерживается на этой системе");
        }
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

        HRESULT comHr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(comHr))
        {
            throw gcnew System::Exception("COM initialization failed: " + hr);
        }
        s_comInitialized = comHr == S_OK;
    }

    void DX12Context::Deinitialize()
    {
        if (s_commandQueue)
        {
            s_commandQueue->Release();
            s_commandQueue = nullptr;
        }
        if (s_device)
        {
            s_device->Release();
            s_device = nullptr;
        }
        if (s_comInitialized)
        {
            CoUninitialize();
            s_comInitialized = false;
        }
        if (s_descriptorHeaps)
        {
            for each (auto heap in s_descriptorHeaps->Values)
            {
                delete heap;
            }
            delete s_descriptorHeaps;
            s_descriptorHeaps = nullptr;
        }
    }

    DX12DescriptorHeap^ DX12Context::GetDescriptorHeap(
        DX12DescriptorHeapType type,
        UINT minCapacity,
        bool shaderVisible
    )
    {
        return DX12DescriptorHeapManager::Instance->GetHeap(
            type, minCapacity, shaderVisible);
    }
}
