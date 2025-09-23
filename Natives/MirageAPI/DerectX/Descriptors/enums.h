#pragma once

namespace MirageAPI::DirectX::Descriptors
{
    public enum class DX12HeapType
    {
        Default = 1,
        Upload = 2,
        Custom = 4,
        Readback = 3
    };

    public enum class DX12DescriptorHeapType
    {
        SRV = 0,
        UAV = SRV,
        Sampler = 1,
        RTV = 2,
        DSV = 3
    };
}
