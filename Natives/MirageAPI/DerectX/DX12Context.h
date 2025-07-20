#pragma once

namespace MirageAPI::DirectX
{
    public ref class DX12Context
    {
    internal:
        static ID3D12Device* s_device;
        static ID3D12Debug* s_debugController;

    public:
        static void Initialize(bool isDebug);
        static void Deinitialize();
    };
}
