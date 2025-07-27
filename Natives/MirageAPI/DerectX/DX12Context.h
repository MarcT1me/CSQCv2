#pragma once

#include "DX12Enums.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Context
    {
    internal:
        static ID3D12Device* s_device;
        static ID3D12Debug* s_debugController;
        static IDXGIFactory4* s_factory;

    public:
        inline static void Initialize(DX12ContextInitFlags flags);
        static void Deinitialize();
    };
}
