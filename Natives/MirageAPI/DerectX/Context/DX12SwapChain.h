#pragma once

#include "DX12ContextConfig.h"
#include "../Command/DX12CommandQueue.h"

namespace MirageAPI::DirectX
{
    public ref class DX12SwapChain : public DX12Object
    {
        IDXGISwapChain3* m_swapChain = nullptr;

        DX12DescriptorHeap^ m_rtvHeap;
        array<Resource::DX12FrameBuffer^>^ m_frameBuffers;
        UINT m_frameIndex = 0;
        Resource::DX12FrameBuffer^ m_currentFrameBuffer;

        DX12ContextConfig^ m_config;

    internal:
        void CreateFrameBuffers();
        void FreeFrameBuffers();

    public:
        DX12SwapChain(HWND hwnd, Command::DX12CommandQueue^ commandQueue, DX12ContextConfig^ config);

        ~DX12SwapChain() { this->!DX12SwapChain(); }
        !DX12SwapChain();

        property DX12DescriptorHeap^ RTVHeap
        {
            DX12DescriptorHeap^ get() { return m_rtvHeap; }
        }
        property Resource::DX12FrameBuffer^ CurrentFrameBuffer
        {
            Resource::DX12FrameBuffer^ get() { return m_currentFrameBuffer; }
        }

        void Resize();
        void Present();
        void UpdateFrameIndex();
        Resource::DX12FrameBuffer^ AcquireNextBackBuffer(Command::DX12CommandList^ commandList);
        void ReleaseBackBufferToPresent(Command::DX12CommandList^ commandList);
    };
}
