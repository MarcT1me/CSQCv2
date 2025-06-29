#pragma once

#include "DX12ContextConfig.h"
#include "CommandList/DX12WindowCommandList.h"
#include "Buffer/DX12FrameBuffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12WindowContext
    {
    internal:
        IDXGISwapChain3* m_swapChain = nullptr;

        ID3D12Fence* m_fence = nullptr;
        HANDLE m_fenceEvent = nullptr;
        UINT64 m_fenceValue = 1;

        array<DX12FrameBuffer^>^ m_frameBuffers;
        DX12WindowCommandList^ m_windowCommandList;
        ID3D12DescriptorHeap* m_rtvHeap = nullptr;

        UINT m_frameIndex = 0;
        UINT m_rtvDescriptorSize = 0;
        UINT m_bufferCount = 2;

        int m_width = 0;
        int m_height = 0;
        int m_vsync = 0;
        DX12WindowContextConfig^ m_config;
        bool disposed = false;

        void CreateFrameBuffers();
        void WaitForGpuCompletion();

    public:
        DX12WindowContext(HWND hwnd, int width, int height,
                          DX12WindowContextConfig^ config);
        ~DX12WindowContext() { this->!DX12WindowContext(); }
        !DX12WindowContext();

        void Resize(int width, int height);
        void SetViewport(float x, float y, float width, float height);
        void SetViewportDepth(float x, float y);
        void BeginFrame();
        void EndFrame();
        void Present();

        property DX12FrameBuffer^ CurrentFrameBuffer
        {
            DX12FrameBuffer^ get()
            {
                return m_frameBuffers != nullptr && m_frameIndex < m_frameBuffers->Length
                           ? m_frameBuffers[m_frameIndex]
                           : nullptr;
            }
        }

        property DX12WindowCommandList^ CommandList
        {
            DX12WindowCommandList^ get() { return m_windowCommandList; }
        }

        property int VSync
        {
            int get() { return m_vsync; }
            void set(int value) { m_vsync = value; }
        }
    };
}
