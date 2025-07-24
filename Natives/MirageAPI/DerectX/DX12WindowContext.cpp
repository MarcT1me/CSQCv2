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
        m_windowCommandList = gcnew DX12WindowCommandList();

        // Создание фабрики DXGI
        IDXGIFactory4* factory = nullptr;
        UINT factoryFlags = config->EnableDebugLayer ? DXGI_CREATE_FACTORY_DEBUG : 0;
        HRESULT hr = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory));
        if (FAILED(hr))
        {
            LPSTR errorText = nullptr;
            FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                hr,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                reinterpret_cast<LPSTR>(&errorText),
                0,
                nullptr
            );

            System::String^ errorMsg;
            if (errorText != nullptr)
            {
                errorMsg = "CreateDXGIFactory2 failed: " + gcnew System::String(errorText);
                LocalFree(errorText);
            }
            else
            {
                errorMsg = "CreateDXGIFactory2 failed with error code: " + hr;
            }

            if (factory) factory->Release();
            throw gcnew System::Exception(errorMsg);
        }

        // Создание swap chain с учетом настроек
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = config->BufferCount;
        swapChainDesc.Width = m_width;
        swapChainDesc.Height = m_height;
        swapChainDesc.Format = static_cast<DXGI_FORMAT>(config->Format);
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        if (config->SwapEffect != DX12SwapEffect::None)
            swapChainDesc.SwapEffect = static_cast<DXGI_SWAP_EFFECT>(config->SwapEffect);
        swapChainDesc.SampleDesc.Count = config->SampleCount;
        swapChainDesc.SampleDesc.Quality = config->SwapQuality;
        swapChainDesc.Flags = config->AllowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

        IDXGISwapChain1* tempSwapChain;
        hr = factory->CreateSwapChainForHwnd(
            m_windowCommandList->NativeQueue,
            hwnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &tempSwapChain
        );

        if (FAILED(hr)) factory->Release();
        DX12_CHECK(device, hr, "CreateSwapChainForHwnd failed");

        IDXGISwapChain3* swapChain;
        hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain));
        tempSwapChain->Release();
        factory->Release();
        DX12_CHECK(device, hr, "QueryInterface for swap chain failed");
        m_swapChain = swapChain;

        // создание буферов кадров
        CreateFrameBuffers();

        // Создание fence
        ID3D12Fence* fence;
        hr = device->CreateFence(
            0,
            D3D12_FENCE_FLAG_NONE,
            IID_PPV_ARGS(&fence)
        );
        DX12_CHECK(device, hr, "CreateFence failed");
        m_fence = fence;

        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!m_fenceEvent)
        {
            throw gcnew System::Exception("Failed to create fence event");
        }
    }

    void DX12WindowContext::CreateFrameBuffers()
    {
        if (!m_swapChain)
            throw gcnew System::InvalidOperationException("Swap chain not initialized");

        auto rtvHeap = gcnew DX12DescriptorHeap(
            DX12DescriptorHeapType::RTV,
            m_bufferCount,
            false
        );
        rtvHeap->Validate();
        m_rtvHeap = rtvHeap;

        // Создаем фрейм-буферы
        m_frameBuffers = gcnew array<DX12FrameBuffer^>(m_bufferCount);
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->NativeHeap->GetCPUDescriptorHandleForHeapStart();

        for (UINT i = 0; i < m_bufferCount; i++)
        {
            // creating resource
            ID3D12Resource* renderTarget = nullptr;
            if (FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget))))
                throw gcnew System::Exception("GetBuffer failed");

            // creating buffer
            m_frameBuffers[i] = gcnew DX12FrameBuffer(
                renderTarget,
                m_width,
                m_height,
                m_config->Format,
                m_rtvHeap,
                i
            );

            // create RTV for heap
            D3D12_RENDER_TARGET_VIEW_DESC desc = {};
            desc.Format = static_cast<DXGI_FORMAT>(m_config->Format);
            desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

            device->CreateRenderTargetView(
                renderTarget,
                &desc,
                m_frameBuffers[i]->RTVHandle
            );

            // attach rtv to heap
            rtvHandle.ptr += m_rtvHeap->DescriptorSize;
        }
    }

    DX12WindowContext::!DX12WindowContext()
    {
        Validate();

        WaitForGpuCompletion();

        FreeFrameBuffers();
        FreeRTVHeap();

        if (m_swapChain)
        {
            m_swapChain->Release();
            m_swapChain = nullptr;
        }

        if (m_fence)
        {
            m_fence->Release();
            m_fence = nullptr;
        }

        if (m_fenceEvent)
        {
            CloseHandle(m_fenceEvent);
            m_fenceEvent = nullptr;
        }

        if (m_windowCommandList != nullptr)
        {
            delete m_windowCommandList;
            m_windowCommandList = nullptr;
        }
    }

    void DX12WindowContext::FreeFrameBuffers()
    {
        if (m_frameBuffers != nullptr)
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
        if (m_rtvHeap != nullptr)
        {
            delete m_rtvHeap;
            m_rtvHeap = nullptr;
        }
    }

    void DX12WindowContext::Resize(int width, int height)
    {
        if (width <= 0 || height <= 0) return;

        WaitForGpuCompletion();

        FreeFrameBuffers();
        FreeRTVHeap();

        m_width = width;
        m_height = height;

        HRESULT hr = m_swapChain->ResizeBuffers(
            m_bufferCount,
            m_width, m_height,
            static_cast<DXGI_FORMAT>(m_config->Format),
            m_config->AllowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0
        );
        DX12_CHECK(device, hr, "Swap chain resize failed");
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

        WaitForGpuCompletion();

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

    void DX12WindowContext::WaitForGpuCompletion()
    {
        if (m_width == 0 || m_height == 0) return;

        const UINT64 fenceValue = m_fenceValue; // save last fence value

        // signal queue
        DX12_CHECK(device, m_windowCommandList->NativeQueue->Signal(m_fence, fenceValue), "Native Queue signal err");
        m_fenceValue++;

        if (m_fence->GetCompletedValue() < fenceValue)
        {
            DX12_CHECK(device, m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent), "Event on completion err");
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }
    }

    void DX12WindowContext::EndFrame()
    {
        if (!m_swapChain || !m_windowCommandList || !m_windowCommandList->NativeList)
            return;

        // Переход в состояние презентации
        CurrentFrameBuffer->TransitionState(m_windowCommandList, DX12ResourceState::Present);
        m_windowCommandList->Close();

        // Выполняем командный список
        m_windowCommandList->Execute();

        m_windowCommandList->NativeQueue->Signal(m_fence, m_fenceValue);
    }

    void DX12WindowContext::Present()
    {
        if (m_width == 0 || m_height == 0) return;

        m_swapChain->Present(m_vsync, 0);

        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

        if (m_fence->GetCompletedValue() < m_fenceValue)
        {
            m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }
        m_fenceValue++;
    }
}
