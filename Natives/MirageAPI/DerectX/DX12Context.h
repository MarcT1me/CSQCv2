#pragma once

#include <d3d12.h>

namespace MirageAPI::DirectX
{
    public ref class DX12Context
    {
        static ID3D12Device* s_device;
        static ID3D12CommandQueue* s_commandQueue;

    public:
        static void Initialize();
        static void Deinitialize();

        static ID3D12Device* GetDevice() { return s_device; }
        static ID3D12CommandQueue* GetCommandQueue() { return s_commandQueue; }
    };
}
