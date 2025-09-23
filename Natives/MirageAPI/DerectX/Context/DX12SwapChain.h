#pragma once

#include "DX12ContextConfig.h"
#include "../Command/DX12CommandQueue.h"

namespace MirageAPI::DirectX
{
    public ref class DX12SwapChain : public DX12Object<DX12ContextConfig^>
    {
        IDXGISwapChain3* _swapChain = nullptr;

        Descriptors::DX12DescriptorHeap^ _rtHeap;
        array<Resource::DX12RenderTarget^>^ _frameBuffers;
        UINT _frameIndex;
        Resource::DX12RenderTarget^ _currentFrameBuffer;

    internal:
        void CreateBuffers();
        void FreeBuffers();

    public:
        DX12SwapChain(
            DX12ContextConfig^ config,
            HWND hwnd,
            Command::DX12CommandQueue^ commandQueue
        );

        ~DX12SwapChain() { this->!DX12SwapChain(); }
        !DX12SwapChain();

        property Descriptors::DX12DescriptorHeap^ RTHeap
        {
            Descriptors::DX12DescriptorHeap^ get() { return _rtHeap; }
        }
        property Resource::DX12RenderTarget^ CurrentFrameBuffer
        {
            Resource::DX12RenderTarget^ get() { return _currentFrameBuffer; }
        }

        void UpdateBufferSizes();
        void UpdateFrameIndex();

        Resource::DX12RenderTarget^ AcquireNextBackBuffer(Command::DX12CommandList^ commandList);
        void ReleaseBackBufferToPresent(Command::DX12CommandList^ commandList);
        void Present();
    };
}
