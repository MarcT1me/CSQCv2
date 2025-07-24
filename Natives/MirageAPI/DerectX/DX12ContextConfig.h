#pragma once

namespace MirageAPI::DirectX
{
    public enum class DX12SwapEffect
    {
        None = 0,
        Discard = DXGI_SWAP_EFFECT_DISCARD,
        FlipDiscard = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        FlipSequential = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
        Sequential = DXGI_SWAP_EFFECT_SEQUENTIAL,
    };

    public value struct DX12WindowContextConfig
    {
        DX12ResourceFormat Format;
        
        bool EnableDebugLayer;
        bool AllowTearing;
        
        UINT m_width;
        UINT m_height;
        UINT m_vsync;
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
                settings.m_width = 0;
                settings.m_height = 0;
                settings.m_vsync = 0;
                
                settings.BufferCount = 2;
                settings.Format = DX12ResourceFormat::RGBA8_UNORM;
                settings.SwapEffect = DX12SwapEffect::FlipDiscard;
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
