#include "pch.h"
#include "DX12WindowContext.h"
#include "DX12Context.h"

// Исправленная реализация с CD3DX12-классами
namespace MirageAPI::DirectX
{
    DX12WindowContext::DX12WindowContext(HWND hwnd, int width, int height)
        : m_width(width), m_height(height)
    {
        // 1. Создание фабрики DXGI
        IDXGIFactory4* factory = nullptr;
        HRESULT hr = CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));
        if (FAILED(hr))
        {
            throw gcnew System::Exception("CreateDXGIFactory2 failed: " + hr);
        }

        // 2. Создание swap chain
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount = 2;
        swapChainDesc.Width = width;
        swapChainDesc.Height = height;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count = 1;

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

        // 3. Создание RTV heap
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = 2;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        ID3D12DescriptorHeap* rtvHeap;
        hr = DX12Context::GetDevice()->CreateDescriptorHeap(
            &rtvHeapDesc,
            IID_PPV_ARGS(&rtvHeap)
        );

        if (FAILED(hr))
        {
            m_swapChain->Release();
            throw gcnew System::Exception("CreateDescriptorHeap failed: " + hr);
        }
        m_rtvHeap = rtvHeap;

        m_rtvDescriptorSize = DX12Context::GetDevice()->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_RTV
        );

        // 4. Создание render targets и RTV
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(
            m_rtvHeap->GetCPUDescriptorHandleForHeapStart()
        );

        // Первый буфер
        ID3D12Resource* renderTarget0;
        hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTarget0));
        if (FAILED(hr))
        {
            Cleanup();
            throw gcnew System::Exception("GetBuffer(0) failed: " + hr);
        }
        m_renderTarget0 = renderTarget0;

        DX12Context::GetDevice()->CreateRenderTargetView(
            m_renderTarget0,
            nullptr,
            rtvHandle
        );
        m_rtvHandle0 = HandleToInt(rtvHandle);

        // Второй буфер
        rtvHandle.ptr += m_rtvDescriptorSize;

        ID3D12Resource* renderTarget1;
        hr = m_swapChain->GetBuffer(1, IID_PPV_ARGS(&renderTarget1));
        if (FAILED(hr))
        {
            Cleanup();
            throw gcnew System::Exception("GetBuffer(1) failed: " + hr);
        }
        m_renderTarget1 = renderTarget1;

        DX12Context::GetDevice()->CreateRenderTargetView(
            m_renderTarget1,
            nullptr,
            rtvHandle
        );
        m_rtvHandle1 = HandleToInt(rtvHandle);

        // 5. Создание командного аллокатора
        ID3D12CommandAllocator* commandAllocator;
        hr = DX12Context::GetDevice()->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(&commandAllocator)
        );
        m_commandAllocator = commandAllocator;

        if (FAILED(hr))
        {
            Cleanup();
            throw gcnew System::Exception("CreateCommandAllocator failed: " + hr);
        }

        // 6. Создание командного списка
        ID3D12GraphicsCommandList* commandList;
        hr = DX12Context::GetDevice()->CreateCommandList(
            0,
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            m_commandAllocator,
            nullptr,
            IID_PPV_ARGS(&commandList)
        );
        m_commandList = commandList;

        if (FAILED(hr))
        {
            Cleanup();
            throw gcnew System::Exception("CreateCommandList failed: " + hr);
        }
        m_commandList->Close();

        // 7. Создание fence
        ID3D12Fence* fence;
        hr = DX12Context::GetDevice()->CreateFence(
            0,
            D3D12_FENCE_FLAG_NONE,
            IID_PPV_ARGS(&fence)
        );

        if (FAILED(hr))
        {
            Cleanup();
            throw gcnew System::Exception("CreateFence failed: " + hr);
        }
        m_fence = fence;

        m_fenceValue = 1;
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!m_fenceEvent)
        {
            Cleanup();
            throw gcnew System::Exception("CreateEvent failed");
        }

        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    }

    DX12WindowContext::~DX12WindowContext()
    {
        Cleanup();
    }

    void DX12WindowContext::Cleanup()
    {
        const UINT64 fence = m_fenceValue;
        DX12Context::GetCommandQueue()->Signal(m_fence, fence);
        m_fenceValue++;

        if (m_fence->GetCompletedValue() < fence)
        {
            m_fence->SetEventOnCompletion(fence, m_fenceEvent);
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }

        if (m_renderTarget0) m_renderTarget0->Release();
        if (m_renderTarget1) m_renderTarget1->Release();
        if (m_swapChain) m_swapChain->Release();
        if (m_commandAllocator) m_commandAllocator->Release();
        if (m_commandList) m_commandList->Release();
        if (m_rtvHeap) m_rtvHeap->Release();
        if (m_fence) m_fence->Release();
        CloseHandle(m_fenceEvent);
    }

    void DX12WindowContext::Resize(int width, int height)
    {
        const UINT64 fence = m_fenceValue;
        DX12Context::GetCommandQueue()->Signal(m_fence, fence);
        m_fenceValue++;
        if (m_fence->GetCompletedValue() < fence)
        {
            m_fence->SetEventOnCompletion(fence, m_fenceEvent);
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }

        m_width = width;
        m_height = height;

        if (m_renderTarget0) m_renderTarget0->Release();
        if (m_renderTarget1) m_renderTarget1->Release();

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
        m_rtvHandle0 = HandleToInt(rtvHandle);
        rtvHandle.ptr += m_rtvDescriptorSize;
        m_rtvHandle1 = HandleToInt(rtvHandle);

        ID3D12Resource* renderTarget0;
        m_swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTarget0));
        m_renderTarget0 = renderTarget0;
        DX12Context::GetDevice()->CreateRenderTargetView(m_renderTarget0, nullptr, IntToHandle(m_rtvHandle0));

        ID3D12Resource* renderTarget1;
        m_swapChain->GetBuffer(1, IID_PPV_ARGS(&renderTarget1));
        m_renderTarget1 = renderTarget1;
        DX12Context::GetDevice()->CreateRenderTargetView(m_renderTarget1, nullptr, IntToHandle(m_rtvHandle1));

        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    }

    void DX12WindowContext::BeginFrame()
    {
        if (m_width == 0 || m_height == 0) return;
        
        // Ждем завершения предыдущего кадра
        const UINT64 fenceValue = m_fenceValue;
        DX12Context::GetCommandQueue()->Signal(m_fence, fenceValue);
        m_fenceValue++;

        if (m_fence->GetCompletedValue() < fenceValue)
        {
            m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }

        m_commandAllocator->Reset();
        m_commandList->Reset(m_commandAllocator, nullptr);

        // Получаем текущий RTV
        D3D12_CPU_DESCRIPTOR_HANDLE currentRtv = GetCurrentRTV();

        ID3D12Resource* currentTarget = GetRenderTarget();

        // Переход в состояние рендеринга
        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = currentTarget;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        m_commandList->ResourceBarrier(1, &barrier);

        // Устанавливаем render target
        m_commandList->OMSetRenderTargets(1, &currentRtv, FALSE, nullptr);
    }

    void DX12WindowContext::EndFrame()
    {
        if (m_width == 0 || m_height == 0) return;
        
        // Получаем текущий RTV
        ID3D12Resource* currentTarget = GetRenderTarget();

        // Переход в состояние презентации
        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = currentTarget;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        m_commandList->ResourceBarrier(1, &barrier);
        m_commandList->Close();

        // Выполняем командный список
        ID3D12CommandList* commandLists[] = {m_commandList};
        DX12Context::GetCommandQueue()->ExecuteCommandLists(_countof(commandLists), commandLists);

        const UINT64 fenceValue = m_fenceValue;
        DX12Context::GetCommandQueue()->Signal(m_fence, fenceValue);
        m_fenceValue++;
    }

    void DX12WindowContext::Present()
    {
        if (m_width == 0 || m_height == 0) return;
        
        m_swapChain->Present(m_vsync, 0);
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

        // Исправить: использовать предыдущее сигнальное значение
        const UINT64 waitValue = m_fenceValue - 1;
        if (m_fence->GetCompletedValue() < waitValue)
        {
            m_fence->SetEventOnCompletion(waitValue, m_fenceEvent);
            WaitForSingleObject(m_fenceEvent, INFINITE);
        }
    }

    void DX12WindowContext::Clear(float r, float g, float b, float a)
    {
        // Получаем текущий RTV
        D3D12_CPU_DESCRIPTOR_HANDLE currentRtv = GetCurrentRTV();

        const float clearColor[] = {r, g, b, a};
        m_commandList->ClearRenderTargetView(currentRtv, clearColor, 0, nullptr);
    }
}
