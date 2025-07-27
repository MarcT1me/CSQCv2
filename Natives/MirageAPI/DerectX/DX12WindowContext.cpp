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
        m_windowCommandList = gcnew CommandList::DX12WindowCommandList();

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
                m_windowCommandList->NativeQueue,
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
        ID3D12Fence* fence;
        CheckHResult(
            device->CreateFence(
                0,
                D3D12_FENCE_FLAG_NONE,
                IID_PPV_ARGS(&fence)
            ),
            "Create Fence failed"
        );
        m_fence = fence;

        // creating fence event
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!m_fenceEvent)
            throw gcnew System::Exception("Failed to create fence event");
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

        WaitForGpuAndSignal();

        if (m_fenceEvent)
        {
            CloseHandle(m_fenceEvent);
            m_fenceEvent = nullptr;
        }

        if (m_fence)
        {
            m_fence->Release();
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

        WaitForGpuAndSignal();

        FreeFrameBuffers();
        FreeRTVHeap();

        m_width = width;
        m_height = height;

        CheckHResult(
            m_swapChain->ResizeBuffers(
                m_bufferCount,
                m_width, m_height,
                static_cast<DXGI_FORMAT>(m_config->Format),
                m_config->AllowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0
            ),
            "Swap chain resize failed"
        );
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

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

        WaitForGpuAndSignal();

        m_windowCommandList->Reset();

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

        // Переход в состояние рендеринга
        CurrentFrameBuffer->TransitionState(m_windowCommandList, DX12ResourceState::RenderTarget);

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CurrentFrameBuffer->RTVHandle;

        m_windowCommandList->NativeList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    }

    void DX12WindowContext::EndFrame()
    {
        if (m_width == 0 || m_height == 0) return;

        // go into present mode
        CurrentFrameBuffer->TransitionState(m_windowCommandList, DX12ResourceState::Present);

        // execute command list
        m_windowCommandList->Close();
        m_windowCommandList->Execute();

        // signalize him
        SignalCommandQueue();
    }

    void DX12WindowContext::Present()
    {
        if (m_width == 0 || m_height == 0) return;

        CheckHResult(m_swapChain->Present(m_vsync, 0), "present swap chain");

        UpdateFrameIndex();

        WaitForGpuCompletion();
        IncreaseFenceValue();
    }

    void DX12WindowContext::WaitForGpuAndSignal()
    {
        SignalCommandQueue();
        IncreaseFenceValue();
        WaitForGpuCompletion();
    }

    void DX12WindowContext::WaitForGpuCompletion()
    {
        if (m_width == 0 || m_height == 0) return;

        // wait for signal if needed
        if (m_fence->GetCompletedValue() < m_fenceValue)
        {
            CheckHResult(
                m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent),
                "Event on completion err"
            );
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }
    }

    void DX12WindowContext::IncreaseFenceValue()
    {
        m_fenceValue++;
    }

    void DX12WindowContext::SignalCommandQueue()
    {
        if (m_width == 0 || m_height == 0) return;

        // signal queue
        CheckHResult(
            m_windowCommandList->NativeQueue->Signal(m_fence, m_fenceValue),
            "Native Queue signal err"
        );
    }

    void DX12WindowContext::UpdateFrameIndex()
    {
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    }
}
