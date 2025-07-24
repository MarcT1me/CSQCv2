#pragma once

#include "DX12ContextConfig.h"

#include "DX12Object.h"
#include "CommandList/DX12WindowCommandList.h"
#include "Buffer/DX12FrameBuffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12WindowContext : public DX12Object
    {
    internal:
        IDXGISwapChain3* m_swapChain = nullptr;

        ID3D12Fence* m_fence = nullptr;
        HANDLE m_fenceEvent = nullptr;
        UINT64 m_fenceValue = 1;

        DX12DescriptorHeap^ m_rtvHeap;
        array<DX12FrameBuffer^>^ m_frameBuffers;
        UINT m_frameIndex = 0;
        UINT m_bufferCount = 2;

        DX12WindowCommandList^ m_windowCommandList;

        int m_width = 0;
        int m_height = 0;
        int m_vsync = 0;
        DX12WindowContextConfig^ m_config;

        void CreateFrameBuffers();
        void WaitForGpuCompletion();

        void FreeFrameBuffers();
        void FreeRTVHeap();

    public:
        DX12WindowContext(
            HWND hwnd,
            int width, int height,
            DX12WindowContextConfig^ config
        );

        ~DX12WindowContext() { this->!DX12WindowContext(); }
        !DX12WindowContext();

        void Resize(int width, int height);
        void SetViewport(float x, float y, float width, float height);
        void SetViewportDepth(float x, float y);
        void Clear(float r, float g, float b, float a);
        void BeginFrame();
        void EndFrame();
        void Present();

        property DX12FrameBuffer^ CurrentFrameBuffer
        {
            DX12FrameBuffer^ get()
            {
                return m_frameBuffers != nullptr && m_frameIndex < static_cast<UINT>(m_frameBuffers->Length)
                           ? m_frameBuffers[m_frameIndex]
                           : nullptr;
            }
        }

        property DX12WindowCommandList^ CommandList { DX12WindowCommandList^ get() { return m_windowCommandList; } }
        property DX12DescriptorHeap^ RTVHeap { DX12DescriptorHeap^ get() { return m_rtvHeap; } }

        property int VSync
        {
            int get() { return m_vsync; }
            void set(int value) { m_vsync = value; }
        }
    };
}
