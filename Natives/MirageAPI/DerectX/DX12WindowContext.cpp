#include "pch.h"
#include "DX12WindowContext.h"

#include "DX12DescriptorHeap.h"

namespace MirageAPI::DirectX
{
    DX12WindowContext::DX12WindowContext(
        HWND hwnd,
        int width, int height,
        DX12WindowContextConfig^ config
    ) : m_bufferCount(config->BufferCount),
        m_width(width),
        m_height(height),
        m_config(config)
    {
        // creating CommandList for window
        m_commandQueue = gcnew CommandList::DX12CommandQueue(DX12CommandListType::Direct);

        // needed in swap chain initializations
        auto factory = GetContextFactory();

        // creating SwapChain
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = config->BufferCount;
        swapChainDesc.Width = m_width;
        swapChainDesc.Height = m_height;
        swapChainDesc.Format = static_cast<DXGI_FORMAT>(config->Format);
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SampleDesc.Count = config->SampleCount;
        swapChainDesc.SwapEffect = static_cast<DXGI_SWAP_EFFECT>(config->SwapEffect);

        if (config->SwapQuality)
            swapChainDesc.SampleDesc.Quality = config->SwapQuality;
        if (config->AllowTearing)
            swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

        IDXGISwapChain1* tempSwapChain;
        CheckHResult(
            factory->CreateSwapChainForHwnd(
                m_commandQueue->NativeQueue,
                hwnd,
                &swapChainDesc,
                nullptr,
                nullptr,
                &tempSwapChain
            ),
            "Create SwapChain failed"
        );

        IDXGISwapChain3* swapChain;
        CheckHResult(
            tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain)),
            "QueryInterface for SwapChain failed"
        );
        tempSwapChain->Release();
        m_swapChain = swapChain;

        // init buffer index
        UpdateFrameIndex();

        // creating FrameBuffers
        CreateFrameBuffers();

        // creating fence
        m_fence = gcnew DX12Fence(1);
        m_commandQueue->Fence = m_fence;

        m_windowCommandList = gcnew CommandList::DX12WindowCommandList();
    }

    void DX12WindowContext::CreateFrameBuffers()
    {
        auto rtvHeap = gcnew DX12DescriptorHeap(
            DX12DescriptorHeapType::RTV,
            m_bufferCount,
            false
        );

        rtvHeap->Validate();
        m_rtvHeap = rtvHeap;

        // creating frame buffer array
        m_frameBuffers = gcnew array<Resource::DX12FrameBuffer^>(m_bufferCount);

        // and buffers
        for (UINT i = 0; i < m_bufferCount; i++)
        {
            // creating resource
            ID3D12Resource* renderTarget = nullptr;
            CheckHResult(
                m_swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget)),
                "GetBuffer failed"
            );

            // creating buffer himself
            auto buffer = gcnew Resource::DX12FrameBuffer(
                renderTarget,
                m_width,
                m_height,
                m_config->Format,
                m_rtvHeap
            );
            // render target view for buffer
            buffer->CreateRTV();
            // save him
            m_frameBuffers[i] = buffer;
        }
    }

    DX12WindowContext::!DX12WindowContext()
    {
        Validate();

        m_commandQueue->Signal();
        m_fence->WaitForCompletion();

        if (m_fence)
        {
            delete m_fence;
            m_fence = nullptr;
        }

        FreeFrameBuffers();
        FreeRTVHeap();

        if (m_swapChain)
        {
            m_swapChain->Release();
            m_swapChain = nullptr;
        }

        if (m_windowCommandList)
        {
            delete m_windowCommandList;
            m_windowCommandList = nullptr;
        }
    }

    void DX12WindowContext::FreeFrameBuffers()
    {
        if (m_frameBuffers)
        {
            for each (auto frameBuffer in m_frameBuffers)
            {
                if (frameBuffer != nullptr)
                    delete frameBuffer;
            }
            m_frameBuffers = nullptr;
        }
    }

    void DX12WindowContext::FreeRTVHeap()
    {
        if (m_rtvHeap)
        {
            delete m_rtvHeap;
            m_rtvHeap = nullptr;
        }
    }

    void DX12WindowContext::Resize(int width, int height)
    {
        if (width <= 0 || height <= 0) return;

        m_commandQueue->WaitForCompletion();
        m_commandQueue->Signal();
        m_fence->WaitForCompletion();
        m_fence->IncreaseValue();

        FreeFrameBuffers();
        FreeRTVHeap();

        m_width = width;
        m_height = height;

        DXGI_SWAP_CHAIN_DESC desc;
        m_swapChain->GetDesc(&desc);

        CheckHResult(
            m_swapChain->ResizeBuffers(
                desc.BufferCount,
                m_width, m_height,
                desc.BufferDesc.Format,
                desc.Flags
            ),
            "Swap chain resize failed"
        );
        UpdateFrameIndex();

        CreateFrameBuffers();
    }

    void DX12WindowContext::SetViewport(float x, float y, float width, float height)
    {
        m_config->viewportX = x;
        m_config->viewportY = y;
        m_config->viewportWidth = width;
        m_config->viewportHeight = height;
    }

    void DX12WindowContext::SetViewportDepth(float x, float y)
    {
        m_config->viewportDepthX = x;
        m_config->viewportDepthY = y;
    }

    void DX12WindowContext::Clear(float r, float g, float b, float a)
    {
        m_windowCommandList->ClearRenderTargetView(CurrentFrameBuffer, r, g, b, a);
    }

    void DX12WindowContext::BeginFrame()
    {
        if (m_width == 0 || m_height == 0) return;

        m_commandQueue->Signal();
        m_fence->WaitForCompletion();
        m_fence->IncreaseValue();

        m_windowCommandList->Reset();

        m_windowCommandList->UpdatePipelineState();
        m_windowCommandList->UpdateDescriptorHeap();

        // first (default) commands
        m_windowCommandList->SetViewport(
            m_config->viewportX, m_config->viewportY,
            m_config->viewportWidth != -1 ? m_config->viewportWidth : static_cast<float>(m_width),
            m_config->viewportHeight != -1 ? m_config->viewportHeight : static_cast<float>(m_height),
            m_config->viewportDepthX, m_config->viewportDepthY
        );
        m_windowCommandList->SetScissorRect(
            0, 0,
            m_width, m_height
        );

        // set frame buffer and go into render mode
        CurrentFrameBuffer->TransitionState(m_windowCommandList, DX12ResourceState::RenderTarget);
        m_windowCommandList->BindBuffer(CurrentFrameBuffer);
    }

    void DX12WindowContext::EndFrame()
    {
        if (m_width == 0 || m_height == 0) return;

        // go into present mode
        CurrentFrameBuffer->TransitionState(m_windowCommandList, DX12ResourceState::Present);
        m_windowCommandList->Close();

        // execute command list and signalize him
        m_commandQueue->ExecuteList(m_windowCommandList);
        m_commandQueue->WaitForCompletion();
        m_commandQueue->Signal();
    }

    void DX12WindowContext::Present()
    {
        if (m_width == 0 || m_height == 0) return;

        CheckHResult(m_swapChain->Present(m_vsync, 0), "present swap chain");

        UpdateFrameIndex();

        m_fence->WaitForCompletion();
        m_fence->IncreaseValue();
    }

    void DX12WindowContext::UpdateFrameIndex()
    {
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    }
}
