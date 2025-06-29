#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

namespace MirageAPI::DirectX
{
    public enum class DX12BufferFormat
    {
        RGBA_UNORM = DXGI_FORMAT_R8G8B8A8_UNORM,
        RGBA_UNORM_SRGB = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        RGBA_FLOAT = DXGI_FORMAT_R16G16B16A16_FLOAT,
        RGB10A2_UNORM = DXGI_FORMAT_R10G10B10A2_UNORM,
    };

    public enum class DX12SwapEffect
    {
        Discard = DXGI_SWAP_EFFECT_DISCARD,
        FlipDiscard = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        FlipSequential = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
        Sequential = DXGI_SWAP_EFFECT_SEQUENTIAL,
        None = 0,
        // Добавьте другие эффекты по необходимости
    };

    public enum class DX12RTVHeapFlags
    {
        None = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
        ShaderVisible = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
    };

    public value struct DX12WindowContextConfig
    {
        DX12BufferFormat Format;
        DX12RTVHeapFlags RTVHeapFlags;
        
        bool EnableDebugLayer;
        bool AllowTearing;
        
        UINT BufferCount;
        DX12SwapEffect SwapEffect;
        UINT SampleCount;
        UINT SwapQuality;

        float viewportX;
        float viewportY;
        float viewportWidth;
        float viewportHeight;
        float viewportDepthX;
        float viewportDepthY;

        static property DX12WindowContextConfig Default
        {
            DX12WindowContextConfig get()
            {
                DX12WindowContextConfig settings;
                settings.BufferCount = 2;
                settings.Format = DX12BufferFormat::RGBA_UNORM;
                settings.SwapEffect = DX12SwapEffect::FlipDiscard;
                settings.RTVHeapFlags = DX12RTVHeapFlags::None;
                settings.SampleCount = 1;
                settings.EnableDebugLayer = false;
                settings.AllowTearing = false;

                settings.viewportX = 0;
                settings.viewportY = 0;
                settings.viewportWidth = -1;
                settings.viewportHeight = -1;
                settings.viewportDepthX = 0;
                settings.viewportDepthY = 1;
                return settings;
            }
        }
    };
}
