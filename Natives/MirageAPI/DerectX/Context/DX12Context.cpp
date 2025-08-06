#include "pch.h"
#include "DX12Context.h"

namespace MirageAPI::DirectX
{
    DX12Context::DX12Context(
        HWND hwnd,
        DX12ContextConfig^ config
    ) : m_config(config)
    {
        m_commandQueue = gcnew Command::DX12CommandQueue(DX12CommandListType::Direct);
        m_swapChain = gcnew DX12SwapChain(hwnd, config, m_commandQueue);
        m_commandQueue->Fence = m_fence = gcnew Command::DX12Fence(1);
        m_commandList = gcnew Command::DX12CommandList(DX12CommandListType::Direct);
    }

    DX12Context::!DX12Context()
    {
        Validate();

        if (m_commandQueue)
        {
            m_commandQueue->Signal();
            m_commandQueue->Wait();
        }
        if (m_fence)
        {
            m_fence->Wait();
        }

        SimpleDelete(m_fence);
        SimpleDelete(m_commandList);
        SimpleDelete(m_commandQueue);
    }

    void DX12Context::SetWinRect(DoubleRect^ winRect)
    {
        m_winRect = winRect;
    }

    void DX12Context::SetResolution(int width, int height)
    {
        m_config->ResolutionX = width;
        m_config->ResolutionY = height;
        m_isResized = true;
    }

    void DX12Context::HandleResize()
    {
        // wait last frame
        m_commandQueue->Wait();
        m_fence->Wait();

        // update swap chain buffers
        m_swapChain->UpdateBufferSizes();
        m_isResized = false;
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

    void DX12Context::BeginFrame()
    {
        if (IncorrectSize) return;

        if (m_isResized) HandleResize();

        m_commandQueue->Signal();

        // prepare command list
        m_commandList->Reset();
        m_commandList->UpdatePipelineState();
        m_commandList->UpdateDescriptorHeap();

        // first (default) commands
        m_commandList->SetViewport(
            m_config->Viewport->X,
            m_config->Viewport->Y,
            m_config->Viewport->Width == -1
                ? static_cast<float>(m_winRect->Width)
                : m_config->Viewport->Width,
            m_config->Viewport->Height == -1
                ? static_cast<float>(m_winRect->Height)
                : m_config->Viewport->Height,
            m_config->Near,
            m_config->Far
        );
        m_commandList->SetScissorRect(
            0, 0,
            m_winRect->Width,
            m_winRect->Height
        );

        // set frame buffer and go into render mode
        m_commandList->BindBuffer(
            m_swapChain->AcquireNextBackBuffer(m_commandList)
        );
    }

    void DX12Context::Clear(float r, float g, float b, float a)
    {
        if (IncorrectSize) return;

        m_commandList->ClearRenderTargetView(m_swapChain->CurrentFrameBuffer, r, g, b, a);
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
