#include "pch.h"
#include "DX12Context.h"

#include "DX12WindowContext.h"

#include <iostream>
#include <ostream>

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
            DX12Context::GetCommandQueue(),
            hwnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &tempSwapChain
        );

        if (FAILED(hr))
        {
            factory->Release();
            throw gcnew System::Exception("CreateSwapChainForHwnd failed: " + hr);
        }

        IDXGISwapChain3* swapChain;
        hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain));
        tempSwapChain->Release();
        factory->Release();

        if (FAILED(hr))
        {
            throw gcnew System::Exception("QueryInterface for swap chain failed: " + hr);
        }
        m_swapChain = swapChain;

        // создание буферов кадров
        CreateFrameBuffers();

        m_windowCommandList = gcnew DX12WindowCommandList();
        m_windowCommandList->Close();

        // Создание fence
        ID3D12Fence* fence;
        hr = DX12Context::GetDevice()->CreateFence(
            0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
        if (FAILED(hr))
        {
            throw gcnew System::Exception("CreateFence failed: " + hr);
        }
        m_fence = fence;

        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!m_fenceEvent)
        {
            throw gcnew System::Exception("Failed to create fence event");
        }
    }

    void DX12WindowContext::CreateFrameBuffers()
    {
        if (!m_swapChain) return;

        if (m_rtvHeap)
        {
            m_rtvHeap->Release();
            m_rtvHeap = nullptr;
        }

        // Создаем RTV heap
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = m_bufferCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        ID3D12DescriptorHeap* rtvHeap = nullptr;
        HRESULT hr = DX12Context::GetDevice()->CreateDescriptorHeap(
            &rtvHeapDesc, IID_PPV_ARGS(&rtvHeap)
        );
        m_rtvHeap = rtvHeap;

        if (FAILED(hr))
        {
            throw gcnew System::Exception("CreateDescriptorHeap failed");
        }

        m_rtvDescriptorSize = DX12Context::GetDevice()->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        // Создаем фрейм-буферы
        m_frameBuffers = gcnew array<DX12FrameBuffer^>(m_bufferCount);
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

        for (UINT i = 0; i < m_bufferCount; i++)
        {
            ID3D12Resource* renderTarget = nullptr;
            if (FAILED(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget))))
            {
                m_rtvHeap->Release();
                throw gcnew System::Exception("GetBuffer failed");
            }

            D3D12_CPU_DESCRIPTOR_HANDLE* handlePtr = new D3D12_CPU_DESCRIPTOR_HANDLE();
            *handlePtr = rtvHandle;

            DX12Context::GetDevice()->CreateRenderTargetView(renderTarget, nullptr, *handlePtr);

            m_frameBuffers[i] = gcnew DX12FrameBuffer(renderTarget, handlePtr);

            rtvHandle.ptr += m_rtvDescriptorSize;
        }
    }

    DX12WindowContext::!DX12WindowContext()
    {
        if (disposed) return;

        WaitForGpuCompletion();

        for each (auto frameBuffer in m_frameBuffers)
        {
            delete frameBuffer;
        }

        delete m_windowCommandList;

        if (m_rtvHeap)
        {
            m_rtvHeap->Release();
            m_rtvHeap = nullptr;
        }

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

        disposed = true;
    }

    void DX12WindowContext::Resize(int width, int height)
    {
        if (width <= 0 || height <= 0) return;

        m_width = width;
        m_height = height;

        WaitForGpuCompletion();

        if (m_rtvHeap)
        {
            m_rtvHeap->Release();
            m_rtvHeap = nullptr;
        }

        for each (auto frameBuffer in m_frameBuffers)
        {
            delete frameBuffer;
        }

        DXGI_SWAP_CHAIN_DESC desc;
        m_swapChain->GetDesc(&desc);

        if (FAILED(m_swapChain->ResizeBuffers(
            m_bufferCount,
            width, height,
            desc.BufferDesc.Format,
            desc.Flags)))
        {
            throw gcnew System::Exception("Swap chain resize failed");
        }
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

        // Получаем текущий RTV
        ID3D12Resource* currentResource = CurrentFrameBuffer->NativeResource;

        // Переход в состояние рендеринга
        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = currentResource;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        m_windowCommandList->NativeCommandList->ResourceBarrier(1, &barrier);

        D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle = CurrentFrameBuffer->RTVHandle;

        m_windowCommandList->NativeCommandList->OMSetRenderTargets(1, rtvHandle, FALSE, nullptr);
    }

    void DX12WindowContext::WaitForGpuCompletion()
    {
        if (m_width == 0 || m_height == 0) return;

        const UINT64 fenceValue = m_fenceValue;
        if (SUCCEEDED(DX12Context::GetCommandQueue()->Signal(m_fence, fenceValue)))
        {
            m_fenceValue++;

            if (m_fence->GetCompletedValue() < fenceValue)
            {
                if (SUCCEEDED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent)))
                {
                    WaitForSingleObject(m_fenceEvent, INFINITE);
                }
            }
        }
    }

    void DX12WindowContext::EndFrame()
    {
        if (!m_swapChain || !m_windowCommandList || !m_windowCommandList->NativeCommandList)
            return;

        // Получаем текущий RTV
        ID3D12Resource* currentResource = CurrentFrameBuffer->NativeResource;

        // Переход в состояние презентации
        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = currentResource;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        m_windowCommandList->NativeCommandList->ResourceBarrier(1, &barrier);
        m_windowCommandList->Close();

        // Выполняем командный список
        ID3D12CommandList* commandLists[] = {m_windowCommandList->NativeCommandList};
        DX12Context::GetCommandQueue()->ExecuteCommandLists(1, commandLists);

        DX12Context::GetCommandQueue()->Signal(m_fence, m_fenceValue);
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
