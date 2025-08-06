#pragma once

#include "DX12ContextConfig.h"
#include "../Command/DX12CommandQueue.h"

namespace MirageAPI::DirectX
{
    public ref class DX12SwapChain : public DX12Object
    {
        IDXGISwapChain3* m_swapChain = nullptr;

        DX12DescriptorHeap^ m_rtvHeap;
        array<Resource::DX12RenderTarget^>^ m_frameBuffers;
        UINT m_frameIndex;
        Resource::DX12RenderTarget^ m_currentFrameBuffer;

        DX12ContextConfig^ m_config;

    internal:
        void CreateBuffers();
        void FreeBuffers();

    public:
        DX12SwapChain(
            HWND hwnd,
            DX12ContextConfig^ config,
            Command::DX12CommandQueue^ commandQueue
        );

        ~DX12SwapChain() { this->!DX12SwapChain(); }
        !DX12SwapChain();

        property DX12DescriptorHeap^ RTVHeap
        {
            DX12DescriptorHeap^ get() { return m_rtvHeap; }
        }
        property Resource::DX12RenderTarget^ CurrentFrameBuffer
        {
            Resource::DX12RenderTarget^ get() { return m_currentFrameBuffer; }
        }

        void UpdateBufferSizes();
        void UpdateFrameIndex();

        Resource::DX12RenderTarget^ AcquireNextBackBuffer(Command::DX12CommandList^ commandList);
        void ReleaseBackBufferToPresent(Command::DX12CommandList^ commandList);
        void Present();
    };
}
