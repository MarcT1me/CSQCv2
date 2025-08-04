#pragma once

namespace MirageAPI::DirectX
{
    public enum class DX12FullscreenScaling
    {
        Unspecified = DXGI_MODE_SCALING_UNSPECIFIED,
        Centered = DXGI_MODE_SCALING_CENTERED,
        Stretch = DXGI_MODE_SCALING_STRETCHED,
    };

    public enum class DX12FullscreenScanlineOrder
    {
        Unspecified = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
        Progressive = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE,
        UpperFieldFirst = DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST,
        LowerFieldFirst = DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST,
    };

    public ref struct DX12FullscreenMode
    {
        bool IsWindowed;
        UINT RefreshNumerator;
        UINT RefreshDenominator;
        DX12FullscreenScaling Scaling;
        DX12FullscreenScanlineOrder ScanlineOrder;

        DX12FullscreenMode(
            bool isWindowed,
            UINT refreshNumerator,
            UINT refreshDenominator,
            DX12FullscreenScaling scaling,
            DX12FullscreenScanlineOrder scanlineOrder
        )
        {
            IsWindowed = isWindowed;
            RefreshNumerator = refreshNumerator;
            RefreshDenominator = refreshDenominator;
            Scaling = scaling;
            ScanlineOrder = scanlineOrder;
        }

        static DX12FullscreenMode^ Windowed(UINT vsyncFrameRate)
        {
            return gcnew DX12FullscreenMode(
                true,
                vsyncFrameRate,
                1,
                DX12FullscreenScaling::Unspecified,
                DX12FullscreenScanlineOrder::Unspecified
            );
        }

        static DX12FullscreenMode^ Desktop(UINT vsyncFrameRate)
        {
            return gcnew DX12FullscreenMode(
                false,
                vsyncFrameRate,
                1,
                DX12FullscreenScaling::Centered,
                DX12FullscreenScanlineOrder::Progressive
            );
        }

    internal:
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC NativeDesk()
        {
            DXGI_SWAP_CHAIN_FULLSCREEN_DESC desc;
            desc.Windowed = IsWindowed;
            desc.RefreshRate = {RefreshNumerator, RefreshDenominator};
            desc.Scaling = static_cast<DXGI_MODE_SCALING>(Scaling);
            desc.ScanlineOrdering = static_cast<DXGI_MODE_SCANLINE_ORDER>(ScanlineOrder);
            return desc;
        }
    };

    public enum class DX12SwapEffect
    {
        Discard = DXGI_SWAP_EFFECT_DISCARD,
        FlipDiscard = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        FlipSequential = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
        Sequential = DXGI_SWAP_EFFECT_SEQUENTIAL,
    };

    public ref struct DX12ContextConfig
    {
        SimpleRect^ Viewport;
        float Near;
        float Far;
        int ResolutionX;
        int ResolutionY;

        UINT BufferCount;
        DX12ResourceFormat Format;

        UINT SampleCount;
        UINT SwapQuality;

        DX12SwapEffect SwapEffect;
        UINT VSyncInterval;

        DX12FullscreenMode^ FullscreenConfig;

    internal:
        DXGI_SWAP_CHAIN_DESC1 NativeSwapChainDesk()
        {
            DXGI_SWAP_CHAIN_DESC1 desc = {};

            desc.BufferCount = BufferCount;
            desc.Width = static_cast<UINT>(ResolutionX);
            desc.Height = static_cast<UINT>(ResolutionY);
            desc.Format = NativeFormat;
            desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            desc.SampleDesc = {SampleCount, SwapQuality};
            desc.SwapEffect = static_cast<DXGI_SWAP_EFFECT>(SwapEffect);
            desc.Flags = Flags;

            return desc;
        }

        property DXGI_FORMAT NativeFormat
        {
            DXGI_FORMAT get() { return static_cast<DXGI_FORMAT>(Format); }
        }

        property UINT Flags
        {
            UINT get()
            {
                bool isWindowed = !FullscreenConfig || FullscreenConfig->IsWindowed;
                return IsDXDebug && isWindowed ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
            }
        }
    };
}
