#include "pch.h"
#include "DX12Context.h"

#include "../Resource/Matrix/DX12FrameBuffer.h"

namespace MirageAPI::DirectX
{
    DX12Context::DX12Context(
        HWND hwnd,
        DX12ContextConfig^ config
    ) : m_config(config)
    {
        // creating CommandList for window
        m_commandQueue = gcnew Command::DX12CommandQueue(DX12CommandListType::Direct);

        m_swapChain = gcnew DX12SwapChain(
            hwnd, m_commandQueue, config
        );

        // creating fence
        m_fence = gcnew Command::DX12Fence(1);
        m_commandQueue->Fence = m_fence;

        m_commandList = gcnew Command::DX12CommandList(DX12CommandListType::Direct);
    }

    DX12Context::!DX12Context()
    {
        Validate();

        if (m_commandQueue)
            m_commandQueue->Signal();
        if (m_fence)
            m_fence->WaitForCompletion();

        SimpleDelete(m_fence);
        SimpleDelete(m_commandList);
        SimpleDelete(m_commandQueue);
    }

    void DX12Context::Resize(UINT width, UINT height)
    {
        m_config->ResolutionX = width;
        m_config->ResolutionY = height;
        if (IncorrectSize) return;

        // wait last frame
        m_commandQueue->Wait();
        m_fence->Wait();

        m_swapChain->FreeFrameBuffers();
        m_swapChain->Resize();
    }

    void DX12Context::SetViewport(float x, float y, float width, float height)
    {
        m_config->Viewport->X = x;
        m_config->Viewport->Y = y;
        m_config->Viewport->Width = width;
        m_config->Viewport->Height = height;
    }

    void DX12Context::SetViewportDepth(float x, float y)
    {
        m_config->Near = x;
        m_config->Far = y;
    }

    void DX12Context::Clear(float r, float g, float b, float a)
    {
        m_commandList->ClearRenderTargetView(m_swapChain->CurrentFrameBuffer, r, g, b, a);
    }

    void DX12Context::BeginFrame()
    {
        if (IncorrectSize) return;

        m_commandQueue->Signal();
        m_fence->Wait();

        m_commandList->Reset();

        m_commandList->UpdatePipelineState();
        m_commandList->UpdateDescriptorHeap();

        // first (default) commands
        m_commandList->SetViewport(
            m_config->Viewport->X,
            m_config->Viewport->Y,
            m_config->Viewport->Width,
            m_config->Viewport->Height,
            m_config->Near,
            m_config->Far
        );
        m_commandList->SetScissorRect(
            0, 0,
            m_config->Viewport->Width,
            m_config->Viewport->Height
        );

        // set frame buffer and go into render mode
        auto frameBuffer = m_swapChain->AcquireNextBackBuffer(m_commandList);
        m_commandList->BindBuffer(frameBuffer);
    }

    void DX12Context::EndFrame()
    {
        if (IncorrectSize) return;

        // go into present mode
        m_swapChain->ReleaseBackBufferToPresent(m_commandList);

        // close command list
        m_commandList->Close();
        // and execute him
        m_commandQueue->ExecuteList(m_commandList);
        m_commandQueue->Wait();
    }

    void DX12Context::Present()
    {
        if (IncorrectSize) return;

        m_swapChain->Present();

        m_fence->Wait();
    }
}
