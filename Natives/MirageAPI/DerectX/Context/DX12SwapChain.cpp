#include "pch.h"
#include "DX12SwapChain.h"

#include "../DX12DescriptorHeap.h"
#include "../Command/DX12CommandQueue.h"
#include "../Resource/Matrix/DX12FrameBuffer.h"

namespace MirageAPI::DirectX
{
    DX12SwapChain::DX12SwapChain(
        HWND hwnd,
        Command::DX12CommandQueue^ commandQueue,
        DX12ContextConfig^ config
    ) : m_config(config)
    {
        // needed in swap chain initializations
        auto factory = DX12Device::GetNativeFactory(true);

        DXGI_SWAP_CHAIN_DESC1 desc = config->NativeSwapChainDesk();
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC* fullscreenDescPtr = nullptr;

        if (config->FullscreenConfig)
        {
            fullscreenDesc = config->FullscreenConfig->NativeDesk();
            fullscreenDescPtr = &fullscreenDesc;
        }

        IDXGISwapChain1* hwndSwapChain;
        CheckHResult(
            factory->CreateSwapChainForHwnd(
                commandQueue->NativeQueue,
                hwnd,
                &desc,
                fullscreenDescPtr,
                nullptr,
                &hwndSwapChain
            ),
            "Create SwapChain failed"
        );

        IDXGISwapChain3* swapChain;
        CheckHResult(
            hwndSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain)),
            "QueryInterface for SwapChain failed"
        );
        hwndSwapChain->Release();
        m_swapChain = swapChain;

        UpdateFrameIndex();

        CreateBuffers();
    }

    void DX12SwapChain::!DX12SwapChain()
    {
        Validate();

        FreeBuffers();

        SimpleRelease(m_swapChain);
    }

    void DX12SwapChain::CreateBuffers()
    {
        m_rtvHeap = gcnew DX12DescriptorHeap(
            DX12DescriptorHeapType::RTV,
            m_config->BufferCount,
            false
        );

        // creating frame buffer array
        m_frameBuffers = gcnew array<Resource::DX12FrameBuffer^>(m_config->BufferCount);

        // and buffers
        for (UINT i = 0; i < m_config->BufferCount; i++)
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
                m_config->ResolutionX,
                m_config->ResolutionY,
                m_config->Format,
                m_rtvHeap
            );
            // render target view for buffer
            buffer->CreateRTV();
            // save him
            m_frameBuffers[i] = buffer;
        }
    }

    void DX12SwapChain::FreeBuffers()
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

        SimpleDelete(m_rtvHeap);
    }

    void DX12SwapChain::UpdateBufferSizes()
    {
        FreeBuffers();

        CheckHResult(
            m_swapChain->ResizeBuffers(
                m_config->BufferCount,
                m_config->ResolutionX,
                m_config->ResolutionY,
                m_config->NativeFormat,
                m_config->Flags
            ),
            "Swap chain resize failed"
        );

        UpdateFrameIndex();

        CreateBuffers();
    }

    void DX12SwapChain::UpdateFrameIndex()
    {
        m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
    }

    Resource::DX12FrameBuffer^ DX12SwapChain::AcquireNextBackBuffer(Command::DX12CommandList^ commandList)
    {
        m_currentFrameBuffer =
            m_frameBuffers != nullptr && m_frameIndex < static_cast<UINT>(m_frameBuffers->Length)
                ? m_frameBuffers[m_frameIndex]
                : throw gcnew DXException("Cant acquire next frame buffer");
        m_currentFrameBuffer->TransitionState(commandList, DX12ResourceState::RenderTarget);
        return m_currentFrameBuffer;
    }

    void DX12SwapChain::ReleaseBackBufferToPresent(Command::DX12CommandList^ commandList)
    {
        m_currentFrameBuffer->TransitionState(commandList, DX12ResourceState::Present);
    }

    void DX12SwapChain::Present()
    {
        CheckHResult(
            m_swapChain->Present(m_config->VSyncInterval, 0),
            "present swap chain"
        );
        UpdateFrameIndex();
    }
}
