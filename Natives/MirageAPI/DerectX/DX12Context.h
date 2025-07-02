#pragma once

#include <d3d12.h>

#include "DX12DescriptorHeap.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Context
    {
        static ID3D12Device* s_device;
        static ID3D12CommandQueue* s_commandQueue;
        static bool s_comInitialized;
        static System::Collections::Generic::Dictionary<DX12DescriptorHeapType, DX12DescriptorHeap^>^ s_descriptorHeaps;

    public:
        static void Initialize(bool isDebug);
        static void Deinitialize();

        static ID3D12Device* GetDevice() { return s_device; }
        static ID3D12CommandQueue* GetCommandQueue() { return s_commandQueue; }

        static DX12DescriptorHeap^ GetDescriptorHeap(
            DX12DescriptorHeapType type,
            UINT minCapacity,
            bool shaderVisible
        );
    };
}
